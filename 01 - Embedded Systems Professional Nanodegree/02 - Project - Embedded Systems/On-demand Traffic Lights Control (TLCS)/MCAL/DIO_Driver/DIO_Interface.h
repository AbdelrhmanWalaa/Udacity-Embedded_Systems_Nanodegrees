/*
 * DIO_Interface.h
 *
 *   Created on: Mar 31, 2021
 *       Author: Abdelrhman Walaa
 *  Description: This file contains all Digital Input Output (DIO) functions' prototypes and definitions (Macros) to avoid magic numbers.
 */

#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

/* The 8 Pins counted from 0 to 7 */
#define DIO_U8_PIN0			0
#define DIO_U8_PIN1			1
#define DIO_U8_PIN2			2
#define DIO_U8_PIN3			3
#define DIO_U8_PIN4			4
#define DIO_U8_PIN5			5
#define DIO_U8_PIN6			6
#define DIO_U8_PIN7			7

/* Pin direction as input = 0 or output = 1 */
#define DIO_U8_PIN_INPUT	0
#define DIO_U8_PIN_OUTPUT	1

/* Pin value as low = 0 or high = 1 */
#define DIO_U8_PIN_LOW		0
#define DIO_U8_PIN_HIGH		1

/* The 4 Ports counted from 0 to 3 */
#define DIO_U8_PORTA		0
#define DIO_U8_PORTB		1
#define DIO_U8_PORTC		2
#define DIO_U8_PORTD		3

/* Port direction as input = 0 or output = 255 */
#define DIO_U8_PORT_INPUT	0
#define DIO_U8_PORT_OUTPUT	255

/* Functions' prototypes */
vd DIO_vdInitialization  ( void );
u8 DIO_u8SetPinDirection ( u8 Cpy_u8PortId, u8 Cpy_u8PinId, u8 Cpy_u8PinDirection );
u8 DIO_u8SetPinValue     ( u8 Cpy_u8PortId, u8 Cpy_u8PinId, u8 Cpy_u8PinValue );
u8 DIO_u8GetPinValue 	 ( u8 Cpy_u8PortId, u8 Cpy_u8PinId, u8 *Cpy_pu8ReturnedPinValue );
u8 DIO_u8TogglePinValue  ( u8 Cpy_u8PortId, u8 Cpy_u8PinId );
u8 DIO_u8SetPortDirection( u8 Cpy_u8PortId, u8 Cpy_u8PortDirection );
u8 DIO_u8SetPortValue	 ( u8 Cpy_u8PortId, u8 Cpy_u8PortValue );
u8 DIO_u8GetPortValue	 ( u8 Cpy_u8PortId, u8 *Cpy_pu8ReturnedPortValue );

#endif /* DIO_INTERFACE_H_ */
