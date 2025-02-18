/*------------------------------------------------------------------------------
 *  Module      : PIR Driver
 *  File        : pir.c
 *  Description : Source file for the PIR driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"
#include "pir.h"

/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/


/* Description:
 * Initialize the PIR pin
 */

void PIR_init(void){
	GPIO_setupPinDirection(PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);
}

/* Description:
 * Read value of PIR pin
 */
uint8 PIR_getValue(void){
	return GPIO_readPin(PIR_PORT_ID ,PIR_PIN_ID);
}
