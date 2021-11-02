/*
 LAB8
 File Name: LCD_Graphics.c
 Author(s): Alex Kremer, Zane Zwanenburg, Rithvik Dyava, Nikhil Krish
 Initial Creation Date: 10/27/2021
 Description: LCD Driver
 Lab Number: 8
 TA: Hassan Iqbal
 Date of last revision: 10/27/2021
 */

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
void ScreenInitialzation(){
	ST7735_InitR(INITR_REDTAB);
}

/*
	Set the screen with the set of data for a corresponding blurb. It is printed row after row leaving a spacing of 2 rows and 3 columns
	Input: pt		Array of strings containing blurb
		   info     Array of corresponding data points
	Output: void
*/
void SetInfographics(char *pt[], uint32_t info[]){
	for(uint8_t i = 0; i<size; i++){
		ST7735_DrawString(i+2,3, pt[i], colorLoop[i+1]);
		ST7735_FillScreen(colorLoop[i+1]);
		ST7735_OutUDec4(uint32_t info[i]);
	}
}


/*
	Set the row with the data for a corresponding blurb. It is printed row specified
	Input: pt		strings containing blurb
		   info     corresponding data points
		   row 		row to be changed
	Output: void
*/
void RowDisplayChange(char *pt, uint32_t info, uint32_t row){
	ST7735_DrawString(row+2,3, pt, colorLoop[row+1]);
	ST7735_FillScreen(colorLoop[row+1]);
	ST7735_OutUDec4(uint32_t info[row]);	
}

/*
	Clears screen with corresponding color
	Input:	color 	The color to fill the new screen
	Output: void
*/
void ClearScreen(uint16_t color){
	ST7735_FillScreen(color);
}
