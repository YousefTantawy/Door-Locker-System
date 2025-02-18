/*------------------------------------------------------------------------------
 *  Module      : PWM Driver
 *  File        : PWM.c
 *  Description : Source file for the PWM driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/

#include "pwm.h"

#include "common_macros.h"
#include "avr/io.h"
#include "gpio.h"

/* Description:
 * Initialize timer0 and start the timer
 * depending on the duty cycle a wave will be given that will be displayed on an Oscillator
 */
void PWM_Timer0_Start(uint8 duty_cycle){

	TCNT0 = 0; // Set initial value

	OCR0 = duty_cycle; // Set compare value

	// GPIO_setupPinDirection(PWM_PORT_ID, PWM_PIN_ID, PIN_OUTPUT);

	DDRB  = DDRB | (1<<PB3);
	/*
	 * FOC0 = 0 to activate PWM mode
	 * WGM01 = 1 & WGM00 = 1 to activate fast PWM
	 * COM01 = 1 & COM00 = 0  to activate NON-INVERTING mode
	 * CS02 = 0 & CS01 = 1 & CS00 = 1 to activate 64 Prescalar
	 */
	TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS01) |  (1 << CS00);
}
