/*------------------------------------------------------------------------------
 *  Module      : Timer Driver
 *  File        : timer.h
 *  Description : Header file for the ATmega32 microcontroller Timer driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*------------------------------------------------------------------------------
 *  							Data Types Declarations
 *----------------------------------------------------------------------------*/

typedef enum {
    TIMER_timer0,
	TIMER_timer1,
	TIMER_timer2
}Timer_ID_Type;

typedef enum{
	NO_CLOCK,
	F_CPU_CLOCK,
	F_CPU_8,
	F_CPU_32_T2,
	F_CPU_64,
	F_CPU_128_T2,
	F_CPU_256,
	F_CPU_1024
}Timer_ClockType;

typedef enum{
    MODE_normal,
	MODE_CTC
}Timer_ModeType;

typedef struct {
	uint16 timer_InitialValue;
	uint32 timer_compare_MatchValue; /*it will be used in compare mode only*/
	Timer_ID_Type timer_ID; /*Choose timer*/
	Timer_ClockType timer_clock; /*Prescalar for timer*/
	Timer_ModeType timer_mode; /*Overflow or CTC mode*/
} Timer_ConfigType;

/*------------------------------------------------------------------------------
 *  							Function Declarations
 *----------------------------------------------------------------------------*/

/*
 * Description:
 * Function to initialize the timer with specific configurations
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description:
 * Function to de-initialize the timer with specific configurations
 */
void Timer_deinit(Timer_ID_Type timer_type);

/*
 * Description:
 * Function to set the callback function for a specific timer
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);


#endif /* TIMER_H_ */
