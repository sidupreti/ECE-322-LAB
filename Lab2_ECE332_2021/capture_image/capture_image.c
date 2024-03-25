#include "address_map_arm.h"
# include "sys/alt_timestamp.h"

#define KEY_BASE              0xFF200050
#define VIDEO_IN_BASE         0xFF203060
#define FPGA_ONCHIP_BASE      0x08000000
#define THRESHOLD 7
int counter = 0;

void B2W(short *Video_Mem_ptr) {
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 320; x++) {
            short colorpixel = *(Video_Mem_ptr + (y << 9) + x);
            // extracting the red green and blue bits from the rgb from the pixel
            int red = (colorpixel >> 11) & 0x1F;
            int green = (colorpixel >> 5) & 0x3F;
            int blue = colorpixel & 0x1F;

            //Converting to grayscale, I gave green twice the weight since it has stronger perception to the eye
            int grayscale = (red + 2*green + blue) / 4;

            //I applied a static threshold to determine if the pixel is black or white 
            short bwPixel; 
            if (grayscale > THRESHOLD) {
                bwPixel = 0xFFFF; // White 
            } else {
                bwPixel = 0x0000; // Black 
            }

            *(Video_Mem_ptr + (y << 9) + x) = bwPixel;
        }
    }
}
void invert(short *Video_Mem_ptr) {
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 320; x++) {
            short colorpixel = *(Video_Mem_ptr + (y << 9) + x);
            // extracting the red green and blue bits from the rgb from the pixel
            int red = (colorpixel >> 11) & 0x1F;
            int green = (colorpixel >> 5) & 0x3F;
            int blue = colorpixel & 0x1F;

            //Converting to grayscale, I gave green twice the weight since it has stronger perception to the eye
            int grayscale = (red + 2*green + blue) / 4;

            //I applied a static threshold to determine if the pixel is black or white 
            short bwPixel; 
            if (grayscale > THRESHOLD) {
                bwPixel = 0x0000; // White 
            } else {
                bwPixel = 0xFFFF; // Black 
            }

            *(Video_Mem_ptr + (y << 9) + x) = bwPixel;
        }
    }
}
void flipX(short *Video_Mem_ptr) {
    for (int y = 0; y < 120; y++) {
        for (int x = 0; x < 320; x++) {
            int mirrorY = 239 - y;
            short temp = *(Video_Mem_ptr + (y << 9) + x);
            *(Video_Mem_ptr + (y << 9) + x) = *(Video_Mem_ptr + (mirrorY << 9) + x);
            *(Video_Mem_ptr + (mirrorY << 9) + x) = temp;
        }
    }
}
void flipY(short *Video_Mem_ptr) {
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 160; x++) {
            int mirrorX = 319 - x;
            short temp = *(Video_Mem_ptr + (y << 9) + x);
            *(Video_Mem_ptr + (y << 9) + x) = *(Video_Mem_ptr + (y << 9) + mirrorX);
            *(Video_Mem_ptr + (y << 9) + mirrorX) = temp;
        }
    }
}



int main(void) {
    volatile int * KEY_ptr = (int *) KEY_BASE;
    volatile int * Video_In_DMA_ptr = (int *) VIDEO_IN_BASE;
    volatile short * Video_Mem_ptr = (short *) FPGA_ONCHIP_BASE;

    *(Video_In_DMA_ptr + 3) = 0x4; // Enable the video
    

    while (1) {
    	 alt_u32 time_now = alt_timestamp();
    	 
    	
        if (*KEY_ptr != 0) { // Check if any KEY was pressed
        	counter++;
            *(Video_In_DMA_ptr + 3) = 0x0; // Disable the video to capture one frame
            while (*KEY_ptr != 0); // Wait for pushbutton KEY release
            alt_printf("Current timestamp: %x\n", time_now);
            break;
        }
    }

    // Convert and display the black and white image
//    B2W((short *)FPGA_ONCHIP_BASE);
//      flipVertical((short *)FPGA_ONCHIP_BASE);
//    	flipHorizontal((short *)FPGA_ONCHIP_BASE);
    	invert((short *)FPGA_ONCHIP_BASE);
}

