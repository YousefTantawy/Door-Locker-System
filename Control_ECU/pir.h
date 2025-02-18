/*------------------------------------------------------------------------------
 *  Module      : PIR Driver
 *  File        : pir.H
 *  Description : Header file for the PIR driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/


#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"

/*------------------------------------------------------------------------------
 * 					 Pre-Processor Constants and Configurations
 *----------------------------------------------------------------------------*/

#define PIR_PORT_ID					PORTC_ID
#define PIR_PIN_ID					PIN2_ID

/*------------------------------------------------------------------------------
 *  							Function Declarations
 *----------------------------------------------------------------------------*/

/* Description:
 * Initialize the PIR pin
 */
void PIR_init(void);

/* Description:
 * Read value of PIR pin
 */
uint8 PIR_getValue(void);


#endif /* PIR_H_ */
