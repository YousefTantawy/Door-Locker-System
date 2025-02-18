/*------------------------------------------------------------------------------
 *  Module      : Motor Driver
 *  File        : Motor.c
 *  Description : Source file for the Motor driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#include "motor.h"
#include "gpio.h"
#include "avr/io.h"

/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/

/*
 * Description:
 * Initialize the Motor pins
 */
void DcMotor_Init(void){

	/*
	 * Set up all ports to be output
	 */
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_EN_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);

	/*
	 * To make sure that Motor is turned off
	 */
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
}

/*
 * Description:
 * Allows the Motor to spin in a certain direction and speed based on user specifications
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed){
	switch(state)
	{
	case 0:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
		break;
	case 1:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
		break;
	case 2:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
		break;
	}

	PWM_Timer0_Start(speed);
}
