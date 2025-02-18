/*------------------------------------------------------------------------------
 *  Module      : PWM Driver
 *  File        : PWM.h
 *  Description : Header file for the PWM driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*------------------------------------------------------------------------------
 * 					 Pre-Processor Constants and Configurations
 *----------------------------------------------------------------------------*/

#define PWM_PORT_ID						PORTB_ID
#define PWM_PIN_ID						PIN3_ID

/*------------------------------------------------------------------------------
 *  							Function Declarations
 *----------------------------------------------------------------------------*/

/* Description:
 * Initialize timer0 and start the timer
 * depending on the duty cycle a wave will be given that will be displayed on an Oscillator
 */
void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_H_ */
