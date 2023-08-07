/*
 * GLI_Private.h
 *
 *   Created on: Apr 25, 2021
 *       Author: Abdelrhman Walaa
 *  Description: This file contains all Global Interrupt (GLI) registers' locations and description.
 *
 */

#ifndef GLI_PRIVATE_H_
#define GLI_PRIVATE_H_

/* Registers' Locations */
#define GLI_U8_SREG_REG		*( ( volatile u8 * ) 0x5F )

/* Registers' Description */
/* The AVR Status Register - SREG: Enable Global Interrupt */
/* Bit 7 -> I: Global Interrupt Enable */
#define GLI_U8_I_BIT			7
/* End of SREG Register */

#endif /* GLI_PRIVATE_H_ */
