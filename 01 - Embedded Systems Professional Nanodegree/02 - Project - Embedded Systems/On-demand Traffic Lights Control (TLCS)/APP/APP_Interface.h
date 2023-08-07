/*
 * APP_Interface.h
 *
 *  Created on: Sep 10, 2022
 *      Author: Abdelrhman Walaa
 * Description: This file contains all Traffic Light Control System (TLCS) Application functions' prototypes and definitions (Macros) to avoid magic numbers.
 */

#ifndef APP_INTERFACE_H_
#define APP_INTERFACE_H_

/* LIB */
#include "../LIB/STD_TYPES/STD_TYPES.h"
#include "../LIB/BIT_MATH/BIT_MATH.h"

/* MCAL */
#include "../MCAL/DIO_Driver/DIO_Interface.h"
#include "../MCAL/GLI_Driver/GLI_Interface.h"
#include "../MCAL/EXI_Driver/EXI_Interface.h"
#include "../MCAL/TMR_Driver/TMR_Interface.h"

/* Traffic Light System Modes */
#define APP_U8_CAR_MODE			0
#define APP_U8_PED_MODE			1

/* Traffic Light System States */
#define APP_U8_TRAFFIC_LIGHT_GREEN		0
#define APP_U8_TRAFFIC_LIGHT_YELLOW		1
#define APP_U8_TRAFFIC_LIGHT_RED		2

#define APP_U8_FLAG_DOWN		0
#define APP_U8_FLAG_UP			1

/* Functions' Prototypes */
vd APP_vdStartProgram      ( void );
vd APP_vdSwitchMode		   ( void );
vd APP_vdSetOVFFlag	       ( void );
vd APP_vdCarMode	       ( void );
vd APP_vdPedestrianMode    ( void );
u8 APP_u8SetCARTrafficLight( u8 Cpy_u8TrafficLight );
u8 APP_u8SetPEDTrafficLight( u8 Cpy_u8TrafficLight );

#endif /* APP_INTERFACE_H_ */
