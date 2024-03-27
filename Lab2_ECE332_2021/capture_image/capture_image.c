 #include "address_map_arm.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
//relevant base addresses
#define KEY_BASE              0xFF200050
#define VIDEO_IN_BASE         0xFF203060
#define FPGA_ONCHIP_BASE      0xC8000000
#define CHAR_BUFFER_BASE      0xC9000000	// Base address for the character buffer

int counter = 0; // Counter for the num of images processed
int t; // Threshold
//declaring functions
void displayTime(void);
void clearText(void);
int getThreshold(short *Video_Mem_ptr);

int main(void)
{       //estbalishing pointers
	volatile int * KEY_ptr			= (int *) KEY_BASE; 
	volatile int * Video_In_DMA_ptr	= (int *) VIDEO_IN_BASE;
	volatile short * Video_Mem_ptr	= (short *) FPGA_ONCHIP_BASE;

	int x, y;

	while (1)
	{
		// BLACK AND WHITE IMAGE ============================================================
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3) = 0x0;			// Disable the video to capture one frame
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
		
		counter++;
		displayTime();
		t = getThreshold((short *)FPGA_ONCHIP_BASE);
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 320; x++) {
				short colorpixel = *(Video_Mem_ptr + (y << 9) + x);
				// extracting the red green and blue bits from the rgb from the pixel
                //applying the correct bit mask
				int red = (colorpixel >> 11) & 0x1F;
				int green = (colorpixel >> 5) & 0x3F;
				int blue = colorpixel & 0x1F;

				//Converting to grayscale, we gave green twice the weight since it has stronger perception to the eye
				int grayscale = (red + 2*green + blue) / 4;

				//Comparing the pixel value to the threshold to detemrine what color the pix should be
				short bwPixel; 
				if (grayscale > t) {
					bwPixel = 0xFFFF; // White 
				} else {
					bwPixel = 0x0000; // Black 
				}

				*(Video_Mem_ptr + (y << 9) + x) = bwPixel;
			}
		}
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
		clearText();
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3)	= 0x4;
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
		
		// INVERTED BLACK AND WHITE IMAGE ============================================================
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3) = 0x0;			// Disable the video to capture one frame
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
		//same process as b to white but now switching the colors
		counter++;
		displayTime();
		t = getThreshold((short *)FPGA_ONCHIP_BASE); 
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 320; x++) {
				short colorpixel = *(Video_Mem_ptr + (y << 9) + x);
				// extracting the red green and blue bits from the rgb from the pixel
                //applying the correct bit mask
				int red = (colorpixel >> 11) & 0x1F;
				int green = (colorpixel >> 5) & 0x3F;
				int blue = colorpixel & 0x1F;

				//Converting to grayscale, I gave green twice the weight since it has stronger perception to the eye
				int grayscale = (red + 2*green + blue) / 4;

				//I applied a static threshold to determine if the pixel is black or white 
				short bwPixel; 
				if (grayscale > t) {
					bwPixel = 0x0000; // White 
				} else {
					bwPixel = 0xFFFF; // Black 
				}

				*(Video_Mem_ptr + (y << 9) + x) = bwPixel;
			}
		}
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
		clearText();
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3)	= 0x4;
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
		
		// VERTICALLY FLIPPED IMAGE ===============================================================
        //only looking at the first half of the image and swapping it with the bottom half, hence the 120
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3) = 0x0;			// Disable the video to capture one frame
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
		
		counter++;
		displayTime();
		for (y = 0; y < 120; y++) {
			for (x = 0; x < 320; x++) {
				int mirrorY = 239 - y;
				short temp = *(Video_Mem_ptr + (y << 9) + x);
				*(Video_Mem_ptr + (y << 9) + x) = *(Video_Mem_ptr + (mirrorY << 9) + x);
				*(Video_Mem_ptr + (mirrorY << 9) + x) = temp;
			}
		}
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
		clearText();
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3)	= 0x4;
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
		
		// HORIZONTALLY FLIPPED IMAGE ===============================================================
        //only looking at the left half of the image and swapping it with the right half, hence the 160
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3) = 0x0;			// Disable the video to capture one frame
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
		
		counter++;
		displayTime();
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 160; x++) {
				int mirrorX = 319 - x;
				short temp = *(Video_Mem_ptr + (y << 9) + x);
				*(Video_Mem_ptr + (y << 9) + x) = *(Video_Mem_ptr + (y << 9) + mirrorX);
				*(Video_Mem_ptr + (y << 9) + mirrorX) = temp;
			}
		}
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
		clearText();
		*(Video_In_DMA_ptr + 3)	= 0x4;				// Enable the video
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				*(Video_In_DMA_ptr + 3)	= 0x4;
				while (*KEY_ptr != 0);				// wait for pushbutton KEY release
				break;
			}
		}
	}
}

// Modified getCurrentTime to display time on the VGA screen using the character buffer
void displayTime(void) {
	// Get current time
	struct tm* timeinfo;
	time_t rawtime = time(NULL);
	timeinfo = localtime( &rawtime );
	
	// Print timestamp
	int x = 1;
	int y = 1;
	int offset;
	char *text = asctime(timeinfo);

	offset = (y << 7) + x; // Calculate the offset based on the x, y position
	while(*text && *text != '\n') {
		*((volatile char*)(CHAR_BUFFER_BASE + offset)) = *(text);
		++text;
		++offset;
	}
	
	// Print image count
	y = 2;
	char counter_str[10];
	sprintf(counter_str, "%d", counter);
	
	offset = (y << 7) + x;
	char *count = counter_str;
	while(*count) {
		*((volatile char*)(CHAR_BUFFER_BASE + offset)) = *(count);
		++count;
		++offset;
	}
}

// Fill the char buffer with spaces
void clearText(void) {
    volatile char *char_buffer = (volatile char *)CHAR_BUFFER_BASE;
    int i;

    // Iterate through the char buffer and set each character to a space
    for (i = 0; i < 80 * 60; ++i) {
        char_buffer[i] = ' ';
    }
}

// Get average grayscale value for black/white threshold
int getThreshold(short *Video_Mem_ptr) {
	int x, y;
	int sum = 0;
	for (y = 0; y < 240; y++) {
		for (x = 0; x < 320; x++) {
			short colorpixel = *(Video_Mem_ptr + (y << 9) + x);
			// extracting the red green and blue bits from the rgb from the pixel
			int red = (colorpixel >> 11) & 0x1F;
			int green = (colorpixel >> 5) & 0x3F;
			int blue = colorpixel & 0x1F;

			//Converting to grayscale, I gave green twice the weight since it has stronger perception to the eye
			int grayscale = (red + 2*green + blue) / 4;
			sum += grayscale;
		}
	}
	return sum/(240*320);
}
