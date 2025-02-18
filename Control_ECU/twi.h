/*------------------------------------------------------------------------------
 *  Module      : TWI Driver
 *  File        : twi.h
 *  Description : Header file for the ATmega32 microcontroller TWI driver
 *  Author      : Yousef Tantawy
 *----------------------------------------------------------------------------*/
#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*------------------------------------------------------------------------------
 * 					 Pre-Processor Constants and Configurations
 *----------------------------------------------------------------------------*/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

typedef enum {
    EEPROM_ADDRESS = 0x00,  // Address for EEPROM
} TWI_AddressType;

typedef enum {
    TWI_BIT_RATE_100KHZ = 0x20,
    TWI_BIT_RATE_400KHZ = 0x02,
	TWI_BIT_RATE_250KHZ = 0x08,
	TWI_BIT_RATE_500KHZ = 0x00
} TWI_BaudRateType;

typedef struct {
    TWI_AddressType address;
    TWI_BaudRateType bit_rate;
} TWI_ConfigType;



/*------------------------------------------------------------------------------
 *  							Function Declarations
 *----------------------------------------------------------------------------*/
void TWI_init(const TWI_ConfigType *Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
