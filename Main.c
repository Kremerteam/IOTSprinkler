/*
 LAB8
 File Name: Main.c
 Author(s): Alex Kremer, Zane Zwanenburg, Rithvik Dyava, Nikhil Krish
 Initial Creation Date: 10/27/2021
 Description: Main Driver for Sprinkler Project
 Lab Number: 8
 TA: Hassan Iqbal
 Date of last revision: 10/27/2021
 */

//========================Main Logic=============================
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "PLL.h"
#include "Clock_LCD.h"
#include "Clock.h"
#include "../inc/Unified_Port_Init.h"
#include "Blynk.h"
#include "Sprinkler.h"

void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s
 
 
 
 int main(void){
	//Initialize Drivers
  PLL_Init(Bus80MHz);   // Bus clock at 80 MHz
  DisableInterrupts();  // Disable interrupts until finished with inits
	Unified_Port_Init();
	SysTick_Init();
	Output_Init();
	Blynk_Init();
  EnableInterrupts();
	 

  while(1) {   
    WaitForInterrupt(); // low power mode	

	}
  
}
