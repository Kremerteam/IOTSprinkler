/*
 LAB11
 File Name: Clock.c
 Author(s): Alex Kremer, Zane Zwanenburg, Rithvik Dyava, Nikhil Krish
 Initial Creation Date: 10/27/2021
 Description: Driver for the clock used by system
 Lab Number: 8
 TA: Hassan Iqbal
 Date of last revision: 10/27/2021
 */

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "Clock.h"
#include "Clock_LCD.h"
#include "Blynk.h"
#include "Sprinkler.h"

static uint8_t systick_count=0;


#define PF1   (*((volatile uint32_t *)0x40025008)) //Light
	
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;            //clear control register
 // NVIC_ST_RELOAD_R = 0x00145AF0;
	//NVIC_ST_RELOAD_R = 0x000C0AF5;
	NVIC_ST_RELOAD_R = 0x000C3500;  //set reload value (60 HZ)
  NVIC_ST_CURRENT_R = 0; 
	NVIC_ST_CTRL_R = 0x07; //Check this if issue
}

//Increases clock every second
void SysTick_Handler(void){
		if(systick_count==99)
		{
			IncrementTime(0);
			PF1 ^= 0x02;				//Heartbeat
			systick_count=0;
		}
			systick_count++;
}
