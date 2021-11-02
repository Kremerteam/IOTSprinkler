//Sprinkler.h
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

extern uint8_t second;
extern uint8_t minute;
extern uint8_t hour;

extern uint8_t timer_second;
extern uint8_t timer_minute;
extern uint8_t timer_hour;

void Start_Sprinkler(void);

void Stop_Sprinkler(void);

bool Get_Sprinkler_On(void);
bool Get_Timer_Set(void);

//type 0 second
//type 1 minute
//type 2 hour

//Increments time of the display clock
void IncrementTime(uint8_t type);

//type 0 second
//type 1 minute
//type 2 hour

//Increments time of the alarm clock
void IncrementTimerTime(uint8_t type);

void Set_Sprinkler_Timer(uint8_t hour, uint8_t minute);

void Clear_Sprinkler(void);

void Set_Duration(uint32_t minutes);



