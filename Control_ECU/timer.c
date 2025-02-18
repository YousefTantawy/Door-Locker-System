/*------------------------------------------------------------------------------
 *  Module      : Timer Driver
 *  File        : timer.c
 *  Description : Source file for the ATmega32 microcontroller Timer driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#include "timer.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */

/*------------------------------------------------------------------------------
 *  							Global Variables
 *----------------------------------------------------------------------------*/

/*
 * Array of pointers to hold the addresses of callback functions for each timer
 */
static volatile void (*g_callBackPtr[3])(void) = {NULL_PTR, NULL_PTR, NULL_PTR};

/*------------------------------------------------------------------------------
 *  							Interrupt Service Routines
 *----------------------------------------------------------------------------*/

ISR(TIMER0_OVF_vect)
{
    if (g_callBackPtr[0] != NULL_PTR) {
        (*g_callBackPtr[0])();
    }
}

ISR(TIMER0_COMP_vect)
{
    if (g_callBackPtr[0] != NULL_PTR) {
        (*g_callBackPtr[0])();
    }
}

ISR(TIMER1_OVF_vect)
{
    if (g_callBackPtr[1] != NULL_PTR) {
        (*g_callBackPtr[1])();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (g_callBackPtr[1] != NULL_PTR) {
        (*g_callBackPtr[1])();
    }
}

ISR(TIMER2_OVF_vect)
{
    if (g_callBackPtr[2] != NULL_PTR) {
        (*g_callBackPtr[2])();
    }
}

ISR(TIMER2_COMP_vect)
{
    if (g_callBackPtr[2] != NULL_PTR) {
        (*g_callBackPtr[2])();
    }
}

/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/

/*
 * Description:
 * Function to initialize the timer with specific configurations
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
    if (Config_Ptr->timer_ID == TIMER_timer0)
    {
        TCNT0 = Config_Ptr->timer_InitialValue;

        /*
         * This calculation is used to determine prescalar since Timer 2 and 0/1 have different values of prescalar
         */
		if(Config_Ptr->timer_clock > 5)
			TCCR0 = (Config_Ptr->timer_clock - 2) | (TCCR0 & 0xF8);
		else if(Config_Ptr->timer_clock > 3)
			TCCR0 = (Config_Ptr->timer_clock - 1) | (TCCR0 & 0xF8);
		else
			TCCR0 = (Config_Ptr->timer_clock) | (TCCR0 & 0xF8);

        TCCR0 |= (1 << FOC0); /* Force Output Compare for non-PWM modes */

        if (Config_Ptr->timer_mode == MODE_normal)
        {
        	/*
        	 * Flag of Normal mode to 1
        	 */
			TIMSK |= 1 << TOIE0;
        }
        else if (Config_Ptr->timer_mode == MODE_CTC)
        {
        	/*
        	 * WGM01 = 1 to activate Compare mode
        	 * Flag of Compare mode to 1
        	 * Set the OCR0 to the inputed compare value
        	 */
        	TCCR0 |= 1 << WGM01;

            OCR0 = Config_Ptr->timer_compare_MatchValue;

            TIMSK |= 1 << OCIE0;
        }
    }

    else if (Config_Ptr->timer_ID == TIMER_timer1)
    {
        TCNT1 = Config_Ptr->timer_InitialValue;

        TCCR1A = 1 << FOC1A; /* Force Output Compare for non-PWM modes */

        /*
         * This calculation is used to determine prescalar since Timer 2 and 0/1 have different values of prescalar
         */
		if(Config_Ptr->timer_clock > 5)
			TCCR1B = (Config_Ptr->timer_clock - 2) | (TCCR1B & 0xF8);
		else if(Config_Ptr->timer_clock > 3)
			TCCR1B = (Config_Ptr->timer_clock - 1) | (TCCR1B & 0xF8);
		else
			TCCR1B = (Config_Ptr->timer_clock) | (TCCR1B & 0xF8);

        if (Config_Ptr->timer_mode == MODE_normal)
        {
        	/*
        	 * Flag of Normal mode to 1
        	 */
        	TIMSK |= 1 << TOIE1;
        }
        else if (Config_Ptr->timer_mode == MODE_CTC)
        {
        	/*
        	 * WGM12 = 1 to activate Compare mode
        	 * Flag of Compare mode to 1
        	 * Set the OCR1A to the inputed compare value
        	 */
            OCR1A = Config_Ptr->timer_compare_MatchValue;
            TIMSK |= 1 << OCIE1A;
        	TCCR1B |= 1 << WGM12;
        }
    }

    else if (Config_Ptr->timer_ID == TIMER_timer2)
    {
		TCNT2 = Config_Ptr->timer_InitialValue;

		/*
		 * Set PreScalar
		 */
		TCCR2 = (Config_Ptr->timer_clock) | (TCCR0 & 0xF8);

		TCCR2 |= (1 << FOC2); /* Force Output Compare for non-PWM modes */

		if (Config_Ptr->timer_mode == MODE_normal)
		{
			/*
			 * Set the Normal mode flag to 1
			 */
			TIMSK |= 1 << TOIE2;
		}
		else if (Config_Ptr->timer_mode == MODE_CTC)
		{
        	/*
        	 * WGM21 = 1 to activate Compare mode
        	 * Flag of Compare mode to 1
        	 * Set the OCR2 to the inputed compare value
        	 */
			TCCR2 |= 1 << WGM21;

			OCR2 = Config_Ptr->timer_compare_MatchValue;

			TIMSK |= 1 << OCIE2;
		}
	}
}

/*
 * Description:
 * Function to de-initialize the timer with specific configurations
 */
void Timer_deinit(Timer_ID_Type timer_type)
{
    if (timer_type == TIMER_timer0)
    {
        TCCR0 = 0x00;
        TCNT0 = 0x00;
        OCR0 = 0x00;
        TIMSK &= ~(1 << TOIE0);
        TIMSK &= ~(1 << OCIE0);
    }
    else if (timer_type == TIMER_timer1)
    {
        TCCR1A = 0x00;
        TCCR1B = 0x00;
        TCNT1 = 0x00;
        OCR1A = 0x00;
        TIMSK &= ~(1 << TOIE1);
        TIMSK &= ~(1 << OCIE1A);
    }
    else if (timer_type == TIMER_timer2)
    {
        TCCR2 = 0x00;
        TCNT2 = 0x00;
        OCR2 = 0x00;
        TIMSK &= ~(1 << TOIE2);
        TIMSK &= ~(1 << OCIE2);
    }
}

/*
 * Description:
 * Function to set the callback function for a specific timer
 * */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
    g_callBackPtr[a_timer_ID] = a_ptr;
}
