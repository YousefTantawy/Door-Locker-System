/*------------------------------------------------------------------------------
 *  Module      : Motor Driver
 *  File        : Motor.h
 *  Description : Header file for the Motor driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"
/*------------------------------------------------------------------------------
 * 					 Pre-Processor Constants and Configurations
 *----------------------------------------------------------------------------*/

#define MOTOR_PORT_ID				PORTD_ID

#define MOTOR_ENABLE_PORT_ID		PORTB_ID

#define MOTOR_EN_PIN_ID				PIN3_ID
#define MOTOR_IN1_PIN_ID			PIN6_ID
#define MOTOR_IN2_PIN_ID			PIN7_ID

/*------------------------------------------------------------------------------
 *  							Data Types Declarations
 *----------------------------------------------------------------------------*/

typedef enum
{
	CW, ACW, STOP
}DcMotor_State;

/*------------------------------------------------------------------------------
 *  							Function Declarations
 *----------------------------------------------------------------------------*/


/*
 * Description:
 * Initialize the Motor pins
 */
void DcMotor_Init(void);

/*
 * Description:
 * Allows the Motor to spin in a certain direction and speed based on user specifications
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);


#endif /* MOTOR_H_ */
