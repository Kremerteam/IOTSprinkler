#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/ST7735.h"
#include "../inc/CortexM.h"
#include "../inc/LaunchPad.h"


int16_t colorLoop[] = {ST7735_RED, ST7735_GREEN, ST7735_BLACK, ST7735_BLUE,  ST7735_CYAN, ST7735_MAGENTA, ST7735_YELLOW, ST7735_WHITE};

/*
	Initialization function that will set the screen to red
	Input: none
	Output: void
*/
void ScreenInitialzation();
/*
	Set the screen with the set of data for a corresponding blurb. It is printed row after row leaving a spacing of 2 rows and 3 columns
	Input: pt		Array of strings containing blurb
		   info     Array of corresponding data points
	Output: void
*/
void SetInfographics(char *pt[], uint32_t info[]);


/*
	Set the row with the data for a corresponding blurb. It is printed row specified
	Input: pt		strings containing blurb
		   info     corresponding data points
		   row 		row to be changed
	Output: void
*/
void RowDisplayChange(char *pt, uint32_t info, uint32_t row);

/*
	Clears screen with corresponding color
	Input:	color 	The color to fill the new screen
	Output: void
*/
void ClearScreen(uint16_t color);
