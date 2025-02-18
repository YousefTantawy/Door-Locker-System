/*------------------------------------------------------------------------------
 *  Module      : Buzzer Driver
 *  File        : buzzer.h
 *  Description : Header file for the Buzzer driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#ifndef BUZZER_H_
#define BUZZER_H_

/*------------------------------------------------------------------------------
 * 					 Pre-Processor Constants and Configurations
 *----------------------------------------------------------------------------*/

#define BUZZER_PORT_ID				PORTC_ID
#define BUZZER_PIN_ID				PIN7_ID

/*------------------------------------------------------------------------------
 *  							Function Declarations
 *----------------------------------------------------------------------------*/

/*
 * Description:
 * This function initializes the buzzer pin to be set to OUTPUT
 */
void BUZZER_init(void);
/*
 * Description:
 * This function sets the buzzer pin to LOGIC_HIGH, hence activating the buzzer
 */
void BUZZER_on(void);
/*
 * Description:
 * This function sets the buzzer pin to LOGIC_LOW, hence activating the buzzer
 */

void BUZZER_off(void);


#endif /* BUZZER_H_ */
