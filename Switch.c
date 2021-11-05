#include <stdint.h>
#include <stdbool.h>
#include "../inc/tm4c123gh6pm.h"


#define PC4   (*((volatile uint32_t *)0x40006040))
#define PF4   (*((volatile uint32_t *)0x40025040)) //Change Mode (LCD), 0x10 is not pressed, 0x00, Reset Switch



void Wait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}


//Handles switch inputs
void Switch_On(void){
	PC4 = 0x10;
}

void Switch_Off(void){
	PC4 = 0x00;
}

void Switches_Init(void){
	SYSCTL_RCGCGPIO_R     |=  0x04;         // Activate clock for Port C
  while((SYSCTL_PRGPIO_R & 0x04) != 0x04){};  // Allow time for clock to start

  GPIO_PORTC_PCTL_R     &= ~0xFFFF0000;   // regular GPIO
  GPIO_PORTC_AMSEL_R    &= ~0xF0;         // disable analog function 
  GPIO_PORTC_DIR_R      |=  0xF0;         // outputs on PC7-PC4
  GPIO_PORTC_AFSEL_R    &= ~0xF0;         // regular port function
  GPIO_PORTC_PUR_R       =  0x00;         // enable pull-up on PC7-PC4
  GPIO_PORTC_DEN_R      |=  0xF0;         // enable digital port

	SYSCTL_RCGCGPIO_R     |= 0x00000020;      // activate clock for Port F
  while((SYSCTL_PRGPIO_R & 0x20)==0){};     // allow time for clock to stabilize

  GPIO_PORTF_LOCK_R     = 0x4C4F434B;       // unlock GPIO Port F
  GPIO_PORTF_CR_R       = 0x1F;             // allow changes to PF4-0

  GPIO_PORTF_AMSEL_R    = 0x00;             // disable analog on PF
  GPIO_PORTF_PCTL_R     = 0x00000000;       // PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R      = 0x0E;             // PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R    = 0x00;             // disable alt funct on PF7-0
  GPIO_PORTF_PUR_R      = 0x11;             // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R      = 0x1F;             // enable digital I/O on PF4-0
}
