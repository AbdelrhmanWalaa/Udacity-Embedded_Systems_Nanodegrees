/*
 * GLI_Program.c
 *
 *   Created on: Apr 25, 2021
 *       Author: Abdelrhman Walaa
 *  Description: This file contains all Global Interrupt (GLI) functions' implementations.
 */

/* LIB */
#include "../../LIB/STD_TYPES/STD_TYPES.h"
#include "../../LIB/BIT_MATH/BIT_MATH.h"

/* MCAL */
#include "GLI_Interface.h"
#include "GLI_Private.h"

/*
 Name: GLI_vdEnableGIE
 Input: void
 Output: void
 Description: Function to set I bit in SREG, to enable the Global Interrupt Enable (GIE).
*/
vd GLI_vdEnableGIE ( void )
{
	SET_BIT( GLI_U8_SREG_REG, GLI_U8_I_BIT );
}

/*
 Name: GLI_vdDisableGIE
 Input: void
 Output: void
 Description: Function to clear I bit in SREG, to disable the Global Interrupt Enable (GIE).
*/
vd GLI_vdDisableGIE( void )
{
	CLR_BIT( GLI_U8_SREG_REG, GLI_U8_I_BIT );
}
