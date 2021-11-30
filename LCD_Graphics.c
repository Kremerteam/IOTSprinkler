/*
 LAB11
 File Name: LCD_Graphics.c
 Author(s): Alex Kremer, Zane Zwanenburg, Rithvik Dyava, Nikhil Krish
 Initial Creation Date: 10/27/2021
 Description: LCD Driver
 Lab Number: 8
 TA: Hassan Iqbal
 Date of last revision: 10/27/2021
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "../inc/ST7735.h"
#include "../inc/CortexM.h"
#include "../inc/LaunchPad.h"
#include "Sprinkler.h"
#include "Blynk.h"


//LCD Display
void LCD_Display(void){
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_SetCursor(0,0);
      Output_Color(ST7735_CYAN);
	
			//Current Time
			ST7735_OutString("Time: ");
			ST7735_OutUDec(hour);
			ST7735_OutString(":");
			ST7735_OutUDec(minute);
      ST7735_OutChar('\n');
			ST7735_OutChar('\n');
	
			//Sprinkler On/Off
			ST7735_OutString("Sprinkler: ");
			if(Get_Sprinkler_On()==1){
				ST7735_OutString("ON");
				ST7735_OutChar('\n');
				ST7735_OutString("Time Remaining: ");
				ST7735_OutUDec(current_duration);
				ST7735_OutString(" Minutes");
			}
			else
				ST7735_OutString("OFF");
			ST7735_OutChar('\n');
			
			//Mode - Manual, Timer, or Light Sensor
			ST7735_OutString("Mode: ");
			
			if(getState()==0) //Manual
				ST7735_OutString("Manual");
			if(getState()==1){ //Timer
				ST7735_OutString("Timer");
				ST7735_OutChar('\n');
				ST7735_OutString("Timer Set");
				if(Get_Timer_Set()==1){
					ST7735_OutString(" at");
					ST7735_OutUDec(timer_hour);
					ST7735_OutString(":");
					ST7735_OutUDec(timer_minute);
				}
				else
					ST7735_OutString(": NO");
			}
			if(getState()==2){ //Light Sensing
				ST7735_OutString("Light Sensor");
			}
			ST7735_OutChar('\n');
			
			//DEBUG
			ST7735_OutString("DEBUG: ");
   //   ST7735_OutUDec(U);
      ST7735_OutChar('\n');
}

/*
void LCD_Graph(){
		//	ST7735_PlotClear(4,127);
			ST7735_XYplotInit("",0,1000,0,300,ST7735_BLACK);
			ST7735_XYplot(1000,MY_X,ActualY,ST7735_BLUE);
			ST7735_XYplot(1000,MY_X,WantedY,ST7735_RED);
}
*/
/*
	Clears screen with corresponding color
	Input:	color 	The color to fill the new screen
	Output: void
*/
void ClearScreen(uint16_t color){
	ST7735_FillScreen(color);
}
