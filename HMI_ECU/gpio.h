/*------------------------------------------------------------------------------
 *  Module      : GPIO Driver
 *  File        : gpio.h
 *  Description : Header file for the ATmega32 microcontroller GPIO driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#ifndef GPIO_H_
#define GPIO_H_


#include "std_types.h"

/*------------------------------------------------------------------------------
 *  				Pre-Processor Constants and Configurations
 *----------------------------------------------------------------------------*/
#define NUM_OF_PORTS           4
#define NUM_OF_PINS_PER_PORT   8

#define PORTA_ID               0
#define PORTB_ID               1
#define PORTC_ID               2
#define PORTD_ID               3

#define PIN0_ID                0
#define PIN1_ID                1
#define PIN2_ID                2
#define PIN3_ID                3
#define PIN4_ID                4
#define PIN5_ID                5
#define PIN6_ID                6
#define PIN7_ID                7

/*------------------------------------------------------------------------------
 *  							Data Types Declarations
 *----------------------------------------------------------------------------*/
typedef enum
{
	PIN_INPUT,PIN_OUTPUT
}GPIO_PinDirectionType;

typedef enum
{
	PORT_INPUT,PORT_OUTPUT=0xFF
}GPIO_PortDirectionType;

/*------------------------------------------------------------------------------
 *  							Function Declarations
 *----------------------------------------------------------------------------*/

/*
 * Description :
 * Set up the DDR of a certain pin (A, B, C or D) to either become an output or an input
 * if an wrong value is given, the function will ignore
 */
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PinDirectionType direction);

/*
 * Description :
 * Change the value of the following pin to either be 1 or 0
 * if an wrong value is given, the function will ignore
 */
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value);

/*
 * Description :
 * Read and return the current of value of the specified pin, which will be either LOGIC_HIGH or LOGIC_LOW
 * if wrong value given, the fucntion will return LOGIC_LOW
 */
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num);

/*
 * Description :
 * Set up the DDR of a certain port (A, B, C or D) to either become an output or an input
 * if an wrong value is given, the function will ignore
 */
void GPIO_setupPortDirection(uint8 port_num, GPIO_PortDirectionType direction);

/*
 * Description :
 * Change the value of the following port to either be 1 or 0
 * if an wrong value is given, the function will ignore
 */
void GPIO_writePort(uint8 port_num, uint8 value);

/*
 * Description :
 * Read and return the current of value of the specified PORT
 * if wrong value given, the fucntion will return LOGIC_LOW
 */
uint8 GPIO_readPort(uint8 port_num);

#endif /* GPIO_H_ */
