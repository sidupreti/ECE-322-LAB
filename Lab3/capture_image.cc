#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include "bmp_utility.h"

#define HW_REGS_BASE (0xff200000)
#define HW_REGS_SPAN (0x00200000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)
#define LED_BASE 0x1000
#define PUSH_BASE 0x3010
#define VIDEO_BASE 0x0000

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240


#define FPGA_ONCHIP_BASE     (0xC8000000)
#define IMAGE_SPAN (IMAGE_WIDTH * IMAGE_HEIGHT * 4)
#define IMAGE_MASK (IMAGE_SPAN - 1)




int main(void) {
    volatile unsigned int *video_in_dma = NULL;
    volatile unsigned int *key_ptr = NULL;
    volatile unsigned short *video_mem = NULL;
    void *virtual_base;
    int fd;

    // Open /dev/mem
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return 1;
    }

    // Map physical memory into virtual address space
    virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);
    if (virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return 1;
    }

    // Calculate the virtual address where our device is mapped
    video_in_dma = (unsigned int *)(virtual_base + ((VIDEO_BASE) & (HW_REGS_MASK)));




    int value = *(video_in_dma+3);

    printf("Video In DMA register updated at:0%x\n",(video_in_dma));

    // Modify the PIO register
    *(video_in_dma+3) = 0x4;
    // *h2p_lw_led_addr = *h2p_lw_led_addr + 1;

    value = *(video_in_dma+3);

    printf("enabled video:0x%x\n",value);

    // Button press
    key_ptr = (unsigned int *)(virtual_base + ((PUSH_BASE) & (HW_REGS_MASK)));	// Mask used to make sure calculated address is within correct range
    while (1) {
        if (*key_ptr != 0x07) {
            *(video_in_dma+3) = 0x0;
            while (*key_ptr != 0x07);
            break;
        }
    }

    value = *(video_in_dma+3);
    printf("disabled video:0x%x\n",value);

    unsigned short pixels[IMAGE_HEIGHT][IMAGE_WIDTH];
    unsigned short pixels_bw[IMAGE_HEIGHT][IMAGE_WIDTH];

    // Set image virtual_base (mmap used to map into virtual memory)
    virtual_base = mmap(NULL, IMAGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, FPGA_ONCHIP_BASE);
    if (virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return 1;
    }

    video_mem = (unsigned short *)(virtual_base + ((FPGA_ONCHIP_BASE) & (IMAGE_MASK)));

    // Get black/white threshold
    int x;
    int y;
    int sum = 0;
    for (y = 0; y < IMAGE_HEIGHT; y++) {
        for (x = 0; x < IMAGE_WIDTH; x++) {
            unsigned short pixel = *(video_mem + (y << 9) + x);

            int red = (pixel >> 11) & 0x1F;
            int green = (pixel >> 5) & 0x3F;
            int blue = pixel & 0x1F;

            int grayscale = (red + 2*green + blue) / 4;
            sum += grayscale;
        }
    }
    int t = sum / (IMAGE_HEIGHT * IMAGE_WIDTH);

    // Read pixel data from FPGA On-chip memory
    for (y = 0; y < IMAGE_HEIGHT; y++) {
        for (x = 0; x < IMAGE_WIDTH; x++) {
            unsigned short pixel = *(video_mem + (y << 9) + x);

            pixels[y][x] = pixel;

            // Get black/white pixel
            int red = (pixel >> 11) & 0x1F;
            int green = (pixel >> 5) & 0x3F;
            int blue = pixel & 0x1F;
            int grayscale = (red + 2*green + blue) / 4;
            if (grayscale > t) {
                pixels_bw[y][x] = 0xFFFF; // White
            } else {
                pixels_bw[y][x] = 0x0000; // Black
            }
        }
    }


    //change
    const char* filename = "final_image_color.bmp";

    // Saving image as color
    saveImageShort(filename,&pixels[0][0],320,240);

    const char* filename1 = "final_image_bw.bmp";
    //saving image as black and white
    saveImageShort(filename1,&pixels_bw[0][0],320,240);


    // Clean up
    if (munmap(virtual_base, IMAGE_SPAN) != 0) {
        printf("ERROR: munmap() failed...\n");
        close(fd);
        return 1;
    }



    close(fd);
    return 0;
}
