/*------------------------------------------------------------------------------
 *  Module      : Application Driver
 *  File        : main.c
 *  Description : Source file for the Application code
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/


#include "lcd.h"
#include "timer.h"
#include "uart.h"
#include "keypad.h"
#include "gpio.h"
#include "common_macros.h"
#include "std_types.h"
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
Timer_ConfigType Timer_Configurations = {0, 0, TIMER_timer2, F_CPU_256, MODE_normal};

/*------------------------------------------------------------------------------
 *  				Global Variables and Function Declarations
 *----------------------------------------------------------------------------*/

/*
 * This is used to indicate if the user has failed to enter the password
 */
static uint8 fail_counter = 0;
/*
 * This variable is to store the keypad number
 */
static uint8 g_key = 100;
/*
 * Store Password
 */
static uint8 g_arrKey[5];
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
 * if timer is configuration overflow, use this in the call back
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
 * This code communicates with the control in order to open the door
 */
void openDoor(void);
/*
 * This code sends password to the control using UART
 */
void sendPass(void);
/*
 * Function to lock system if the user enters password wrong 3 times
 */
void lockSystem(void);
/*
 * Function to recieve system password from the user for the first time, if returns 0 if both passworrds don't match, and 1 if they do
 */
uint8 firstPass(void);
/*
 * Same as the firstPass function but different name
 */
void changePass(void);
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

int main()
{
	/*
	 * Global Interrupt Flag
	 */
	SREG = 1 << 7;

	/*
	 * Driver Initializations
	 */
	UART_init(&UART_Configurations);
	LCD_init();
	/*
	 * Start by Presenting on the screen the Project name
	 */
	LCD_displayString("Door System Lock");
	_delay_ms(2000);

	/*
	 * This infinite loop exists to allow the user to enter first system password as much as needed with no errors
	 */
	for(;;)
	{

		if(firstPass())
		{
			/* Do nothing*/
		}
		else
		{
			break;
		}
	}

	/*
	 * This infinite loop is for the system.
	 * has 2 options: Open door or Change Password
	 */
	for(;;)
	{
		/*
		 * Always display these 2 options after every
		 */
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		LCD_displayString("+ : OPEN DOOR");
		LCD_moveCursor(1,0);
		LCD_displayString("- : Change Pass");

		g_key = 100;

		/*
		 * While the keys + and - are not pressed, stay here
		 */
		while(g_key != '+' && g_key != '-')
		{
			_delay_ms(350);
			g_key = KEYPAD_getPressedKey();
		}

		/*
		 * Code for opening door
		 */
		if(g_key == '+')
		{
			/*
			 * Send to control that the open door function has been chosen
			 */
			UART_sendByte(OPENDOOR);
			/*
			 * Enter password and send it over to make sure it is correct
			 */
			for(fail_counter = 0; fail_counter < 3; fail_counter++)
			{
				LCD_clearScreen();
				LCD_displayString("Enter Old Pass:");
				LCD_moveCursor(1,0);
				sendPass();

				while(UART_recieveByte() != READY);

				if(UART_recieveByte())
				{
					/* Do nothing*/
				}
				else
				{
					break;
				}
			}

			if(fail_counter == 3)
			{
				/*
				 * Function to lock system
				 */
				lockSystem();
			}
			else
			{
				/*
				 * Function to open door
				 */
				openDoor();
			}
		}
		/*
		 * Code for changing password
		 */
		else if(g_key == '-')
		{
			/*
			 * Send to control that the change password function has been chosen
			 */
			UART_sendByte(CHANGEPASS);
			/*
			 * Enter password and send it over to make sure it is correct
			 */
			for(fail_counter = 0; fail_counter < 3; fail_counter++)
			{
				LCD_clearScreen();
				LCD_displayString("Enter Old Pass:");
				LCD_moveCursor(1,0);
				sendPass();

				while(UART_recieveByte() != READY);

				if(UART_recieveByte())
				{
					/* Do nothing*/
				}
				else
				{
					break;
				}
			}

			if(fail_counter == 3)
			{
				/*
				 * Function to lock system
				 */
				lockSystem();
			}
			else
			{
				/*
				 * Function to change password
				 */
				changePass();
			}
		}
	}
}



/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/

void sendPass()
{
	/*
	 * This for loop is for entering a Password of 5 integers and storing them in an array
	 */
	for(uint8 count = 0; count < 5; count++)
	{
		g_key = 100;
		while((g_key > 9) || (g_key < 0))
		{
			_delay_ms(350);
			g_key = KEYPAD_getPressedKey();
		}

		LCD_displayCharacter('*');   /* display the pressed keypad switch */

		g_arrKey[count] = g_key;
	}

	/*
	 * Wait for the user to press the enter key then start the transmission using UART
	 */
	while(g_key != KEYPAD_ENTER_KEY)
	{
		_delay_ms(350);
		g_key = KEYPAD_getPressedKey();
	}

	UART_sendByte(READY);

	for(uint8 i = 0; i < 5; i++)
	{
		_delay_ms(50);
		UART_sendByte(g_arrKey[i]);
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



	LCD_clearScreen();
	LCD_moveCursor(0,3);
	LCD_displayString("Door Opening");
	LCD_moveCursor(1,4);
	LCD_displayString("Please Wait");

	/*
	 * Display until the timer activates flag, which is in 15 seconds
	 */
	while(!g_flag);

	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Wait for people");
	LCD_moveCursor(1,3);
	LCD_displayString("to enter");

	while(UART_recieveByte() != READY);

	/*
	 * Wait till people pass
	 */
	while(UART_recieveByte() != NO_PEOPLE);

	g_flag = 0;
	g_tick = 0;

	LCD_clearScreen();
	LCD_moveCursor(0,2);
	LCD_displayString("Door Closing");
	LCD_moveCursor(1,4);
	LCD_displayString("Please Wait");

	/*
	 * Display until the timer activates flag, which is in 15 seconds
	 */
	while(!g_flag);

	Timer_deinit(Timer_Configurations.timer_ID);
}

void lockSystem()
{
	timerCalculations();

	_delay_ms(100);
	calc = (LOCKTIME*CPU_FREQ)/(timerMode_num * timerClock_num);
	/*
	 * Start the timer and set callback function
	 */

	Timer_setCallBack(timer_callBack, Timer_Configurations.timer_ID);
	Timer_init(&Timer_Configurations);
	g_tick = 0;
	g_flag = 0;

	LCD_clearScreen();
	LCD_moveCursor(0,2);
	LCD_displayString("SYSTEM LOCKED");
	LCD_moveCursor(1,0);
	LCD_displayString("Wait 1 minute");

	/*
	 * Wait for timer to raise flag, 60 seconds
	 */
	while(!g_flag);

	Timer_deinit(Timer_Configurations.timer_ID);
}

uint8 firstPass(void)
{
	LCD_clearScreen();
	LCD_displayString("Enter Pass:");
	LCD_moveCursor(1,0);

	sendPass();

	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Re-Enter Pass:");
	LCD_moveCursor(1,0);

	sendPass();

	LCD_clearScreen();

	/*
	 * Wait for signal which indicates whether the passwords match or don't
	 * if yes, exit the for loop, which in this case is the function of else
	 */
	while(UART_recieveByte() != READY);

	return UART_recieveByte();
}


void changePass()
{
	for(;;)
	{
		if(firstPass())
		{
			/* Do nothing*/
		}
		else
		{
			break;
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
