/*------------------------------------------------------------------------------
 *  Module      : Buzzer Driver
 *  File        : buzzer.c
 *  Description : Source file for the Buzzer driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#include "common_macros.h"
#include "buzzer.h"
#include "gpio.h"

/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/

/*
 * Description:
 * This function initializes the buzzer pin to be set to OUTPUT
 */
void BUZZER_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
}
/*
 * Description:
 * This function sets the buzzer pin to LOGIC_HIGH, hence activating the buzzer
 */
void BUZZER_on(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}
/*
 * Description:
 * This function sets the buzzer pin to LOGIC_LOW, hence activating the buzzer
 */

void BUZZER_off(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}


