/*------------------------------------------------------------------------------
 *  Module      : Application Driver
 *  File        : main.c
 *  Description : Source file for the Application code
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#include "buzzer.h"
#include "std_types.h"
#include "common_macros.h"
#include "external_eeprom.h"
#include "gpio.h"
#include "motor.h"
#include "pir.h"
#include "pwm.h"
#include "timer.h"
#include "twi.h"
#include "uart.h"
#include "avr/io.h"
#include "util/delay.h"

/*------------------------------------------------------------------------------
 *  				Pre-Processor Constants and Configurations
 *----------------------------------------------------------------------------*/

#define READY 0x01
#define REPEAT 0x02
#define NO_REPEAT 0x00
#define OPENDOOR 0x03
#define CHANGEPASS 0x04
#define LOCKSYSTEM 0x05
#define NO_PEOPLE 0x06

#define CPU_FREQ 8000000
#define DOORTIME 15
#define LOCKTIME 60
/*
 * Driver configurations
 */
UART_ConfigType UART_Configurations = {UART_8_BITS, UART_NO_PARITY, UART_ONE_STOP_BIT, UART_BAUD_9600};
TWI_ConfigType TWI_Configurations = {EEPROM_ADDRESS, TWI_BIT_RATE_400KHZ};
Timer_ConfigType Timer_Configurations = {0, 0, TIMER_timer2, F_CPU_256, MODE_normal};



/*------------------------------------------------------------------------------
 *  				Global Variables and Function Declarations
 *----------------------------------------------------------------------------*/

/*
 * This flag is used to indicate if a certain amount of ticks has been reached
 * In this codes case, 15 and 60 seconds
 */
static uint8 g_flag = 0;
/*
 * Ticks of timers
 */
static uint32 g_tick = 0;
/*
 * Array to store the password of
 */
static uint8 Pass[5];
/*
 * Value read from the EEPROM when comparing passwords
 */
static uint8 EEPROM_val;
/*
 * This is used to indicate if the user has failed to enter the password
 */
static uint8 fail_counter = 0;
/*
 * Status variable to exit or stay in loop
 */
static uint8 status = 1;
/*
 * if timer is configuration overflow, use this in the call back, otherwise it is equal to ctc number
 */
static uint32 timerMode_num;
/*
 * take number of prescalar to use in timer equation
 */
static uint32 timerClock_num;
/*
 * this variable calculates the amount of ticks needed to reach,
 */
static uint32 calc;

/*
 * Receive Password from the HMI MC
 */
void recievePass(void);

/*
 * Function to activate Motor, which resembles opening the door
 */
void openDoor();

/*
 * Function to lock system if the user enters password wrong 3 times
 */
void lockSystem();

/*
 * Function to recieve system password from the user for the first time, if returns 0 if both passworrds don't match, and 1 if they do
 */
uint8 firstPass(void);

/*
 * Same as the firstPass function but different name
 */
void changePass(void);

/*
 * Compares the passwords, be aware that it compares bytes so you must have it in for loop to work
 */
uint8 comparePasswords(uint8 Val, uint8 Val2);

/*
 * This is the call-back function for the timer driver
 */
void timer_callBack(void)
{
	g_tick++;
	if(g_tick == calc)
	{
		g_tick = 0;
		g_flag = 1;
	}
}
/*
 * This function is used to calculate the ticks needed for any prescalar, any mode and any timer
 */
void timerCalculations();

/*------------------------------------------------------------------------------
 *  						Application Code
 *----------------------------------------------------------------------------*/
int main() {

	/*
	 * Activate the global interrupt register
	 */
	SREG = 1 << 7;

	/*
	 * Initialize all drivers
	 */
	UART_init(&UART_Configurations);
	BUZZER_init();
	TWI_init(&TWI_Configurations);
	DcMotor_Init();
	PIR_init();

	/*
	 * This first for loop is for the user entering the first password of the system, it will not break if the passwords are incorrect which
	 * means it will keep looping forever
	 */
	for(;;)
	{
		/*
		 * If status is one, meaning the passwords are matching, send to the other MC that there is no need to repeat
		 * the process and we can move on to the main system
		 */
		if(firstPass())
		{
			UART_sendByte(READY);
			_delay_ms(50);
			UART_sendByte(NO_REPEAT);
			break;
		}
		else
		{
			UART_sendByte(READY);
			_delay_ms(50);
			UART_sendByte(REPEAT);
		}
	}

	/*
	 * This infinite for loop activates after the first Password, it stays on for as long as the system is on
	 * if the byte received is + which indicates opening the door, it performs that code
	 * else, it will be - which is changing the password
	 */
	for(;;)
	{
		if(UART_recieveByte() == OPENDOOR)
		{
			/*
			 * This block of code is used to compare the password received and the password stored in the EEPROM
			 */
			for(fail_counter = 0; fail_counter < 3; fail_counter++)
			{
				recievePass();
				for(uint8 i = 0; i < 5; i++)
				{
					EEPROM_readByte(EEPROM_ADDRESS+i, &EEPROM_val);
					_delay_ms(50);
					if(comparePasswords(Pass[i], EEPROM_val))
					{
						/*
						 * Break the loop since the bytes don't match and reset
						 */
						status = 0;
						break;
					}
					else
					{
						status = 1;
					}
				}

				/*
				 * If status is one, meaning the passwords are matching, send to the other MC that there is no need to repeat
				 * the process and we can move on
				 */
				if(status)
				{
					UART_sendByte(READY);
					_delay_ms(50);
					UART_sendByte(NO_REPEAT);
					break;
				}
				else
				{
					UART_sendByte(READY);
					_delay_ms(50);
					UART_sendByte(REPEAT);
				}
			}

			if(fail_counter == 3)
			{
				/*
				 * Lock system since failure to enter the correct password is 3
				 */
				lockSystem();
			}
			else
			{
				/*
				 * The password is correct and therefore opening the door starts
				 */
				openDoor();
			}
		}

		else
		{
			/*
			 * This block of code is used to compare the password received and the password stored in the EEPROM
			 */
			for(fail_counter = 0; fail_counter < 3; fail_counter++)
			{
				recievePass();
				for(uint8 i = 0; i < 5; i++)
				{
					EEPROM_readByte(EEPROM_ADDRESS+i, &EEPROM_val);
					_delay_ms(50);
					if(comparePasswords(Pass[i], EEPROM_val))
					{
						/*
						 * Break the loop since the bytes don't match and reset
						 */
						status = 0;
						break;
					}
					else
					{
						status = 1;
					}
				}

				/*
				 * If status is one, meaning the passwords are matching, send to the other MC that there is no need to repeat
				 * the process and we can move on
				 */
				if(status)
				{
					UART_sendByte(READY);
					_delay_ms(50);
					UART_sendByte(NO_REPEAT);
					break;
				}
				else
				{
					UART_sendByte(READY);
					_delay_ms(50);
					UART_sendByte(REPEAT);
				}
			}

			if(fail_counter == 3)
			{
				/*
				 * Lock system since failure to enter the correct password is 3
				 */
				lockSystem();
			}
			else
			{
				/*
				 * The password is correct and therefore changing password starts
				 */
				changePass();
			}
		}
	}
}




/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/


uint8 comparePasswords(uint8 Val, uint8 Val2)
{
	if (Val != Val2) // Passwords do not match
	{
		return 1;
	}
    return 0; // Passwords match
}


void recievePass(){
	/*
	 * Wait for the HMI to be ready
	 */
	while(UART_recieveByte() != READY);

	for(uint8 i = 0; i < 5; i++)
	{
		_delay_ms(50);
		Pass[i] = UART_recieveByte();
		// Receive the password
	}
}

void openDoor()
{
	timerCalculations();

	_delay_ms(100);
	calc = (DOORTIME*CPU_FREQ)/(timerMode_num * timerClock_num);
	/*
	 * Start the timer and set callback function
	 */
	g_tick = 0;
	g_flag = 0;
	Timer_setCallBack(timer_callBack, Timer_Configurations.timer_ID);
	Timer_init(&Timer_Configurations);

	/*
	 * Rotate until the timer activates flag, which is in 15 seconds
	 */
	DcMotor_Rotate(CW, 255);
	while(!g_flag);
	DcMotor_Rotate(STOP, 255);

	/*
	 * Wait till people pass
	 */
	while(PIR_getValue())

	_delay_ms(50);

	/*
	 * Tell the HMI that people have passed and it is ready to close the door
	 */
	UART_sendByte(READY);

	UART_sendByte(NO_PEOPLE);

	g_flag = 0;
	g_tick = 0;

	/*
	 * Rotate anti-clockwise until the timer activates flag, which is in 15 seconds
	 */
	DcMotor_Rotate(ACW, 255);
	while(!g_flag);
	DcMotor_Rotate(STOP, 255);
	Timer_deinit(Timer_Configurations.timer_ID);
}

void lockSystem()
{
	timerCalculations();

	_delay_ms(100);
	calc = (LOCKTIME*CPU_FREQ)/(timerMode_num * timerClock_num);

	/*
	 * Activate buzzer alarm
	 */
	BUZZER_on();
	/*
	 * Start the timer and set callback function
	 */

	Timer_setCallBack(timer_callBack, Timer_Configurations.timer_ID);
	Timer_init(&Timer_Configurations);
	g_tick = 0;
	g_flag = 0;
	/*
	 * Wait for timer to raise flag, 60 seconds
	 */
	while(!g_flag);

	Timer_deinit(Timer_Configurations.timer_ID);
	/*
	 * deactivate buzzer alarm
	 */
	BUZZER_off();
}

uint8 firstPass(void)
{
	recievePass();

	/*
	 * This following block of code is to receive and write the password in the eeprom
	 * if incorrect, the for loop is exited and the process is reset
	 */
	while(UART_recieveByte() != READY);

	for(uint8 i = 0; i < 5; i++)
	{
		_delay_ms(50);
		if(comparePasswords(Pass[i], UART_recieveByte()))
		{
			/*
			 * Break the loop since the bytes don't match and reset
			 */
			status = 0;
			return status;
		}
		else
		{
			status = 1;
			EEPROM_writeByte(EEPROM_ADDRESS+i,Pass[i]);
		}
	}
	return status;
}

void changePass()
{
	/*
	 * This infinite for loop is for the user entering the password of the system forever until it is correct
	 */
	for(;;)
	{
		/*
		 * If status is one, meaning the passwords are matching, send to the other MC that there is no need to repeat
		 * the process and we can move on to the main system
		 */
		if(firstPass())
		{
			UART_sendByte(READY);
			_delay_ms(50);
			UART_sendByte(NO_REPEAT);
			break;
		}
		else
		{
			UART_sendByte(READY);
			_delay_ms(50);
			UART_sendByte(REPEAT);
		}
	}
}

void timerCalculations()
{
	timerMode_num = Timer_Configurations.timer_compare_MatchValue - Timer_Configurations.timer_InitialValue;

	if(Timer_Configurations.timer_mode == MODE_normal)
	{
		if(Timer_Configurations.timer_ID == TIMER_timer1)
		{
			timerMode_num = 65535 - Timer_Configurations.timer_InitialValue;
		}
		else
		{
			timerMode_num = 255 - Timer_Configurations.timer_InitialValue;
		}
	}

	_delay_ms(100);

	switch(Timer_Configurations.timer_clock)
	{
		case NO_CLOCK:
			timerClock_num = 0;
			break;
		case F_CPU_CLOCK:
			timerClock_num = 1;
			break;
		case F_CPU_8:
			timerClock_num = 8;
			break;
		case F_CPU_32_T2:
			timerClock_num = 32;
			break;
		case F_CPU_64:
			timerClock_num = 64;
			break;
		case F_CPU_128_T2:
			timerClock_num = 128;
			break;
		case F_CPU_256:
			timerClock_num = 256;
			break;
		case F_CPU_1024:
			timerClock_num = 1024;
			break;
	}
}
