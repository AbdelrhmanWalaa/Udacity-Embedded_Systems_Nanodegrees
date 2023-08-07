/*
 * EXI_Interface.h
 *
 *   Created on: Apr 25, 2021
 *       Author: Abdelrhman Walaa
 *  Description: This file contains all External Interrupt (EXI) functions' prototypes and definitions (Macros) to avoid magic numbers.
 */
 
#ifndef EXI_INTERFACE_H_
#define EXI_INTERFACE_H_

/* The 3 External Interrupts counted from 0 to 2 */
#define EXI_U8_INT0						0
#define EXI_U8_INT1 					1
#define EXI_U8_INT2 					2

/* Interrupts Sense Control */
#define EXI_U8_SENSE_LOW_LEVEL			0
#define EXI_U8_SENSE_LOGICAL_CHANGE		1
#define EXI_U8_SENSE_FALLING_EDGE		2
#define EXI_U8_SENSE_RISING_EDGE		3

/* Functions' Prototypes */
u8 EXI_u8EnablePIE  ( u8 Cpy_u8InterruptId, u8 Cpy_u8SenseControl );
u8 EXI_u8DisablePIE ( u8 Cpy_u8InterruptId );
u8 EXI_u8SetCallBack( u8 Cpy_u8InterruptId, void ( *Cpy_pfvdInterruptAction ) ( void ) );

#endif /* EXTI_INTERFACE_H_ */
