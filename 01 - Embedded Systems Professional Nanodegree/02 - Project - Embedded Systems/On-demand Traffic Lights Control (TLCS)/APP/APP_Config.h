/*
 * APP_Config.h
 *
 *  Created on: Sep 10, 2022
 *      Author: Abdelrhman Walaa
 * Description: This file contains all Traffic Light Control System (TLCS) Application pre-build configurations, through which user can configure before using the TLCS Application.
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

/* The Port connected to LEDs */
/* Options: DIO_U8_PORTA
			DIO_U8_PORTB
			DIO_U8_PORTC
			DIO_U8_PORTD
 */
#define APP_U8_CAR_LED_PORT			DIO_U8_PORTA
#define APP_U8_PED_LED_PORT			DIO_U8_PORTB

/* The Pins connected to LEDs */
/* Options: DIO_U8_PIN0
			DIO_U8_PIN1
			DIO_U8_PIN2
			DIO_U8_PIN3
			DIO_U8_PIN4
			DIO_U8_PIN5
			DIO_U8_PIN6
			DIO_U8_PIN7
 */
#define APP_U8_CAR_GREEN_LED		DIO_U8_PIN0
#define APP_U8_CAR_YELLOW_LED		DIO_U8_PIN1
#define APP_U8_CAR_RED_LED			DIO_U8_PIN2

#define APP_U8_PED_GREEN_LED		DIO_U8_PIN0
#define APP_U8_PED_YELLOW_LED		DIO_U8_PIN1
#define APP_U8_PED_RED_LED			DIO_U8_PIN2

#endif /* APP_CONFIG_H_ */
