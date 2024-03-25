#include "address_map_arm.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_stdio.h"

#define KEY_BASE              0xFF200050
#define VIDEO_IN_BASE         0xFF203060
#define FPGA_ONCHIP_BASE      0xC8000000
#define THRESHOLD 7

int counter = 0;

/* This program demonstrates the use of the D5M camera with the DE1-SoC Board
 * It performs the following: 
 * 	1. Capture one frame of video when any key is pressed.
 * 	2. Display the captured frame when any key is pressed.		  
*/
/* Note: Set the switches SW1 and SW2 to high and rest of the switches to low for correct exposure timing while compiling and the loading the program in the Altera Monitor program.
*/
int main(void)
{
	volatile int * KEY_ptr				= (int *) KEY_BASE;
	volatile int * Video_In_DMA_ptr	= (int *) VIDEO_IN_BASE;
	volatile short * Video_Mem_ptr	= (short *) FPGA_ONCHIP_BASE;

	int x, y;

	alt_printf("Picture count: %d", counter);
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
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 320; x++) {
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
		alt_printf("Picture count: %d", counter);
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
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
		
		counter++;
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 320; x++) {
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
		alt_printf("Picture count: %d", counter);
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
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
		for (y = 0; y < 120; y++) {
			for (x = 0; x < 320; x++) {
				int mirrorY = 239 - y;
				short temp = *(Video_Mem_ptr + (y << 9) + x);
				*(Video_Mem_ptr + (y << 9) + x) = *(Video_Mem_ptr + (mirrorY << 9) + x);
				*(Video_Mem_ptr + (mirrorY << 9) + x) = temp;
			}
		}
		alt_printf("Picture count: %d", counter);
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
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
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 160; x++) {
				int mirrorX = 319 - x;
				short temp = *(Video_Mem_ptr + (y << 9) + x);
				*(Video_Mem_ptr + (y << 9) + x) = *(Video_Mem_ptr + (y << 9) + mirrorX);
				*(Video_Mem_ptr + (y << 9) + mirrorX) = temp;
			}
		}
		alt_printf("Picture count: %d", counter);
		
		while (1)
		{
			if (*KEY_ptr != 0)						// check if any KEY was pressed
			{
				break;
			}
		}
		
		// RESUME VIDEO =============================================================
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
