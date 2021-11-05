/*
 LAB8
 File Name: Sprinkler.c
 Author(s): Alex Kremer, Zane Zwanenburg, Rithvik Dyava, Nikhil Krish
 Initial Creation Date: 10/27/2021
 Description: Sprinkler Driver
 Lab Number: 8
 TA: Hassan Iqbal
 Date of last revision: 10/27/2021
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Sprinkler.h"
#include "Switch.h"

static uint32_t sprinkler_duration = 30;
static uint8_t sprinkler_state = 0;

//Declare Time Variables
uint8_t second=0;
uint8_t minute=0;
uint8_t hour=0;

uint8_t timer_second=0;
uint8_t timer_minute=0;
uint8_t timer_hour=0;

uint8_t duration=30;
uint8_t current_duration=30;

//Declare Status Variables
static uint8_t timer_set = 0;
static uint8_t sprinkler_on = 0;

void Start_Sprinkler(){
	Switch_On();
	sprinkler_on = 1;
}

void Stop_Sprinkler(){
	Switch_Off();
	sprinkler_on = 0;
}

uint8_t Get_Sprinkler_On(){
	return sprinkler_on;
}

uint8_t Get_Timer_Set(){
	return timer_set;
}

//type 0 second
//type 1 minute
//type 2 hour

//Increments time of the display clock
void IncrementTime(uint8_t type)
{
		if(type==0){ //Second
			second++;
			if(second==60)
			{
				second=0;
				minute++;
				if(minute==60)
				{
					minute=0;
					hour++;
					if(hour==24)
					{
						hour=0;
					}
				}
			}
		}
		if(type==1){ //Minute
			minute++;
				if(minute==60)
				{
					minute=0;
					hour++;
					if(hour==24)
					{
						hour=0;
					}
				}
		}
		if(type==2){ //Hour
			hour++;
			if(hour==24)
			{
				hour=0;
			}
		}
		if(sprinkler_on){
			current_duration--;
			if(current_duration==0)
			{
				Stop_Sprinkler();
				current_duration=duration;
			}			
		}
		if(timer_set && timer_hour==hour && timer_minute==minute)
				Start_Sprinkler();	
}

//type 0 second
//type 1 minute
//type 2 hour

//Increments time of the alarm clock
void IncrementTimerTime(uint8_t type)
{
		if(type==0){ //Second
			timer_second++;
			if(timer_second==60)
			{
				timer_second=0;
				timer_minute++;
				if(timer_minute==60)
				{
					timer_minute=0;
					timer_hour++;
					if(timer_hour==24)
					{
						timer_hour=0;
					}
				}
			}
		}
		if(type==1){ //Minute
			timer_minute++;
				if(timer_minute==60)
				{
					timer_minute=0;
					timer_hour++;
					if(timer_hour==24)
					{
						timer_hour=0;
					}
				}
		}
		if(type==2){ //Hour
			timer_hour++;
			if(timer_hour==24)
			{
				timer_hour=0;
			}
		}
		
}

void Set_Sprinkler_Timer(uint8_t hour, uint8_t minute){
	timer_hour = hour;
	timer_minute = minute;
	timer_set = 1;
}

void Clear_Sprinkler(){
	timer_hour = 0;
	timer_minute = 0;
	timer_set = 0;
}

void Set_Duration(uint32_t minutes){
	sprinkler_duration = minutes;
}


