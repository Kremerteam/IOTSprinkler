/*
 LAB8
 File Name: Blynk.c
 Author(s): Alex Kremer, Zane Zwanenburg, Rithvik Dyava, Nikhil Krish
 Initial Creation Date: 10/27/2021
 Description: WIFI Driver
 Lab Number: 8
 TA: Hassan Iqbal
 Date of last revision: 10/27/2021
 */

// TM4C123       ESP8266-ESP01 (2 by 4 header)
// PE5 (U5TX) to Pin 1 (Rx)
// PE4 (U5RX) to Pin 5 (TX)
// PE3 output debugging
// PE2 nc
// PE1 output    Pin 7 Reset
// PE0 input     Pin 3 Rdy IO2
//               Pin 2 IO0, 10k pullup to 3.3V  
//               Pin 8 Vcc, 3.3V (separate supply from LaunchPad 
// Gnd           Pin 4 Gnd  
// Place a 4.7uF tantalum and 0.1 ceramic next to ESP8266 3.3V power pin
// Use LM2937-3.3 and two 4.7 uF capacitors to convert USB +5V to 3.3V for the ESP8266
// http://www.ti.com/lit/ds/symlink/lm2937-3.3.pdf
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "PLL.h"
#include "Timer2.h"
#include "Timer3.h"
#include "UART.h"
#include "PortF.h"
#include "esp8266.h"
#include "Speaker.h"
#include "Clock_LCD.h"
#include "Lab3_Systick.h"
#include "../inc/Unified_Port_Init.h"

void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

uint32_t LED;      // VP1
uint32_t LastF;    // VP74
// These 6 variables contain the most recent Blynk to TM4C123 message
// Blynk to TM4C123 uses VP0 to VP15
char serial_buf[64];
char Pin_Number[2]   = "99";       // Initialize to invalid pin number
char Pin_Integer[8]  = "0000";     //
char Pin_Float[8]    = "0.0000";   //
uint32_t pin_num; 
uint32_t pin_int;



//================From Lab 3=================================
void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}

void Pause(void){
  while(PF4==0x00){ 
    DelayWait10ms(10);
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}

//Declare Time Variables
static uint8_t second=0;
static uint8_t minute=0;
static uint8_t hour=0;

static uint8_t stop_millisecond=0;
static uint8_t stop_second=0;
static uint8_t stop_minute=0;
static uint8_t stop_hour=0;

static uint8_t alarm_second=1;
static uint8_t alarm_minute=1;
static uint8_t alarm_hour=0;

//Declare Status Variables
static bool alarm = false;
static bool alarm_set = false;
static bool refresh = false;

//Turns alarm on
void trigger_alarm(void)
{
	alarm=true;
}

//Updates clock display
void refresh_clock(void)
{
	refresh=true;
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
		
		if(alarm_hour==hour && alarm_minute==minute)
				trigger_alarm();		
}

//type 0 second
//type 1 minute
//type 2 hour

//Increments time of the alarm clock
void IncrementAlarmTime(uint8_t type)
{
		if(type==0){ //Second
			alarm_second++;
			if(alarm_second==60)
			{
				alarm_second=0;
				alarm_minute++;
				if(alarm_minute==60)
				{
					alarm_minute=0;
					alarm_hour++;
					if(alarm_hour==24)
					{
						alarm_hour=0;
					}
				}
			}
		}
		if(type==1){ //Minute
			alarm_minute++;
				if(alarm_minute==60)
				{
					alarm_minute=0;
					alarm_hour++;
					if(alarm_hour==24)
					{
						alarm_hour=0;
					}
				}
		}
		if(type==2){ //Hour
			alarm_hour++;
			if(alarm_hour==24)
			{
				alarm_hour=0;
			}
		}
		
}

//Increments time of the stopwatch
void IncrementStopTime(void)
{
			stop_millisecond++;
			if(stop_millisecond==100){
				stop_millisecond=0;
				stop_second++;
				if(stop_second==60)
				{
					stop_second=0;
					stop_minute++;
					if(stop_minute==60)
					{
						stop_minute=0;
						stop_hour++;
						if(stop_hour==24)
						{
							stop_hour=0;
						}
					}
				}
		}
}

//Resets stop watch to 0s
void ResetStopWatch(void)
{
	stop_hour=0;
	stop_minute=0;
	stop_second=0;
	stop_millisecond=0;
}

//Adds another minute to alarm when it goes off if toggled
void SnoozeAlarm(void)
{
	if(alarm)
	{
		Sound_Off();
		alarm_set=true;
		alarm=false;
		alarm_minute+=1;
	}
	
}
		
//Declare state logic
static uint8_t state = 0;
static uint8_t old_state=0;
static uint8_t stopwatch=0;

//Allows drivers to access main state value
int getState(void)
{
	return state;
}

//Goes to a new state 0,1,2,3
void ChangeMode()
{
		state = (state+1)%4;
}

//Used when PC7 pressed. Turns off alarm, resets to state 0, or toggles stopwatch
void Confirm() //PC7
{
	alarm = false;
	if(state==2){
		state=0;
		alarm_set=true;
	}
	if(state==1)
		state=0;
	if(state==3)
		stopwatch ^=1;
		
}

int getStopWatch(void)
{
	return stopwatch;
}


// ----------------------------------- TM4C_to_Blynk ------------------------------
// Send data to the Blynk App
// It uses Virtual Pin numbers between 70 and 99
// so that the ESP8266 knows to forward the data to the Blynk App
void TM4C_to_Blynk(uint32_t pin,uint32_t value){
  if((pin < 70)||(pin > 99)){
    return; // ignore illegal requests
  }
// your account will be temporarily halted 
  // if you send too much data
  ESP8266_OutUDec(pin);        // Send the Virtual Pin #
  ESP8266_OutChar(',');
  ESP8266_OutUDec(value);      // Send the current value
  ESP8266_OutChar(',');
  ESP8266_OutString("0.0\n");  // Null value not used in this example
}
 
 
// -------------------------   Blynk_to_TM4C  -----------------------------------
// This routine receives the Blynk Virtual Pin data via the ESP8266 and parses the
// data and feeds the commands to the TM4C.
void Blynk_to_TM4C(void){int j; char data;
// Check to see if a there is data in the RXD buffer
  if(ESP8266_GetMessage(serial_buf)){  // returns false if no message
    // Read the data from the UART5
#ifdef DEBUG1
    j = 0;
    do{
      data = serial_buf[j];
      UART_OutChar(data);        // Debug only
      j++;
    }while(data != '\n');
    UART_OutChar('\r');        
#endif
           
// Rip the 3 fields out of the CSV data. The sequence of data from the 8266 is:
// Pin #, Integer Value, Float Value.
    strcpy(Pin_Number, strtok(serial_buf, ","));
    strcpy(Pin_Integer, strtok(NULL, ","));       // Integer value that is determined by the Blynk App
    strcpy(Pin_Float, strtok(NULL, ","));         // Not used
    pin_num = atoi(Pin_Number);     // Need to convert ASCII to integer
    pin_int = atoi(Pin_Integer);  
  // ---------------------------- VP #1 ----------------------------------------
  // This VP is the LED select button
    if(pin_num == 0x01)  {  
      LED = pin_int;
      PortF_Output(LED<<2); // Blue LED
		}
		if(pin_num == 0x03 && pin_int==1 && state==1) //LOOK AT PIN_INT
			IncrementTime(1); //Increment Clock Minute by 1
		if(pin_num == 0x03 && pin_int==1 && state==2)
			IncrementAlarmTime(1); //Increment Alarm Minute by 1
		if(pin_num == 0x02 && pin_int==1 && state==1)
			IncrementTime(2); //Increment Clock by 1 Hour
		if(pin_num == 0x02 && pin_int==1 && state==2)
			IncrementAlarmTime(2); //Increment Alarm Hour by 1
		if(pin_num==0x05 && pin_int==1)
			Confirm(); //Confirm and return to home clock
		if(pin_num == 0x01 && pin_int==1)
			ChangeMode(); //Increase state by 1
		if(pin_num == 0x04 && pin_int==1 && state==3)
			ResetStopWatch(); //Sets stopwatch to 0
		if(pin_num == 0x04 && pin_int==1)
			SnoozeAlarm(); //Adds a minute to alarm
#ifdef DEBUG3
      Output_Color(ST7735_CYAN);
      ST7735_OutString("Rcv VP1 data=");
      ST7735_OutUDec(LED);
      ST7735_OutChar('\n');
#endif
                                  // Parse incoming data        
#ifdef DEBUG1
    UART_OutString(" Pin_Number = ");
    UART_OutString(Pin_Number);
    UART_OutString("   Pin_Integer = ");
    UART_OutString(Pin_Integer);
    UART_OutString("   Pin_Float = ");
    UART_OutString(Pin_Float);
    UART_OutString("\n\r");
#endif
  }  
}

bool send_info = false;

void SendInformation(void){
/*  uint32_t thisF;
  thisF = PortF_Input();
// your account will be temporarily halted if you send too much data
  if(thisF != LastF){
    TM4C_to_Blynk(74, thisF);  // VP74
#ifdef DEBUG3
    Output_Color(ST7735_WHITE);
    ST7735_OutString("Send 74 data=");
    ST7735_OutUDec(thisF);
    ST7735_OutChar('\n');
#endif
  }
  LastF = thisF;*/
	if(send_info)
	{
		send_info=false;
		//Difference output to Blynk based on state
		if(state==0 || state==1)
		{
			TM4C_to_Blynk(74, hour);
			TM4C_to_Blynk(75, minute);
			TM4C_to_Blynk(76, second);
			TM4C_to_Blynk(77, 0);
		}
		else if(state==2)
		{
			TM4C_to_Blynk(74, alarm_hour);
			TM4C_to_Blynk(75, alarm_minute);
			TM4C_to_Blynk(76, alarm_second);
			TM4C_to_Blynk(77, 0);
		}
		else if(state==3)
		{
			TM4C_to_Blynk(74, stop_hour);
			TM4C_to_Blynk(75, stop_minute);
			TM4C_to_Blynk(76, stop_second);
			TM4C_to_Blynk(77, stop_millisecond);
		}
			
}
		
	
}

//========================Main Logic=============================
 
int main(void){       
  PLL_Init(Bus80MHz);   // Bus clock at 80 MHz
  DisableInterrupts();  // Disable interrupts until finished with inits
  PortF_Init();
	Port_C_Init();
	Port_B_Init();
	Output_Init();
	SysTick_Init();
	Speaker_Init();
	
  LastF = PortF_Input();
#ifdef DEBUG3
  Output_Init();        // initialize ST7735
  ST7735_OutString("EE445L Lab 4D\nBlynk example\n");
#endif
#ifdef DEBUG1
  UART_Init(5);         // Enable Debug Serial Port
  UART_OutString("\n\rEE445L Lab 4D\n\rBlynk example");
#endif
  ESP8266_Init();       // Enable ESP8266 Serial Port
  ESP8266_Reset();      // Reset the WiFi module
  ESP8266_SetupWiFi();  // Setup communications to Blynk Server  
  
  Timer2_Init(&Blynk_to_TM4C,800000); 
  // check for receive data from Blynk App every 10ms

  Timer3_Init(&SendInformation,40000000); 
  // Send data back to Blynk App every 1/2 second
  EnableInterrupts();

  while(1) {   
    //WaitForInterrupt(); // low power mode
		if(alarm && alarm_set){
		Sound_On();
		alarm_set=false;
	}
	//Turn off alarm sound
	if(!alarm)
		Sound_Off();
	//Logic to render state on LCD
	if(old_state!=state || refresh){
		send_info=true;
		if(refresh)
			refresh=false;
		if(old_state!=state){
			old_state=state;
			ST7735_FillScreen(ST7735_BLACK);
		}
		if(state==0){
			RenderClock(hour,minute,second);
		}
		else if(state==1)
			RenderSetClock(hour,minute,second);
		else if(state==2)
			RenderSetAlarm(alarm_hour,alarm_minute,alarm_second);
		else if(state==3){
			RenderStopWatch(stop_hour,stop_minute,stop_second,stop_millisecond);
		}
	}

	}
  
}
