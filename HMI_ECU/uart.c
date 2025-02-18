/*------------------------------------------------------------------------------
 *  Module      : UART Driver
 *  File        : uart.c
 *  Description : Source file for the ATmega32 microcontroller UART driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*------------------------------------------------------------------------------
 *  							Function Definitions
 *----------------------------------------------------------------------------*/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */

void UART_init(const UART_ConfigType * Config_Ptr)
{
    uint16 ubrr_value = 0;

    /* U2X = 1 for double transmission speed */
    UCSRA = (1<<U2X);

    /* Enable Receiver and Transmitter */
    UCSRB = (1<<RXEN) | (1<<TXEN);

    /* UCSRC settings - URSEL must be 1 to write to UCSRC */
    UCSRC = (1<<URSEL);

    /* Set the number of data bits */
    switch(Config_Ptr->bit_data)
    {
        case UART_5_BITS:
            break;
        case UART_6_BITS:
        	UCSRC |= 1 << UCSZ0;
            break;
        case UART_7_BITS:
        	UCSRC |= 1 << UCSZ1;
            break;
        case UART_8_BITS:
        	UCSRC |= 1 << UCSZ1;
        	UCSRC |= 1 << UCSZ0;
            break;
        case UART_9_BITS:
        	UCSRC |= 1 << UCSZ1;
        	UCSRC |= 1 << UCSZ0;
        	UCSRC |= 1 << UCSZ2;
            break;
    }

    /* Set parity mode */
    switch(Config_Ptr->parity)
    {
        case UART_NO_PARITY:
            break;
        case UART_EVEN_PARITY:
        	UCSRC |= 1 << UPM1;
            break;
        case UART_ODD_PARITY:
        	UCSRC |= 1 << UPM0;
        	UCSRC |= 1 << UPM1;
            break;
    }

    /* Set stop bit(s) */
    if(Config_Ptr->stop_bit == UART_TWO_STOP_BITS)
    {
       	UCSRC |= 1 << USBS;
    }

    /* Calculate the UBRR value for the given baud rate */
    ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);

    /* Set the baud rate */
    UBRRH = ubrr_value >> 8;
    UBRRL = ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
