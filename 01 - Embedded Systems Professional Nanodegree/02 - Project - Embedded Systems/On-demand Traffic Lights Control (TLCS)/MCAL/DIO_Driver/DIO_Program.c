/*
 * DIO_Interface.h
 *
 *   Created on: Mar 31, 2021
 *       Author: Abdelrhman Walaa
 *  Description: This file contains all Digital Input Output (DIO) functions' implementations.
 */

/* LIB */
#include "../../LIB/STD_TYPES/STD_TYPES.h"
#include "../../LIB/BIT_MATH/BIT_MATH.h"

/* MCAL */
#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "DIO_Config.h"

/*
 Name: DIO_vdInitialization
 Input: void
 Output: void
 Description: Function to Initialize DIO peripheral.
*/
vd DIO_vdInitialization  ( void )
{
	/* Set Initial Port Direction */
	/* PORTA */
	DIO_U8_DDRA_REG = DIO_U8_CONC( DIO_U8_PA7_INITIAL_DIRECTION, DIO_U8_PA6_INITIAL_DIRECTION, DIO_U8_PA5_INITIAL_DIRECTION, DIO_U8_PA4_INITIAL_DIRECTION, DIO_U8_PA3_INITIAL_DIRECTION, DIO_U8_PA2_INITIAL_DIRECTION, DIO_U8_PA1_INITIAL_DIRECTION, DIO_U8_PA0_INITIAL_DIRECTION );
	/* PORTB */
	DIO_U8_DDRB_REG = DIO_U8_CONC( DIO_U8_PB7_INITIAL_DIRECTION, DIO_U8_PB6_INITIAL_DIRECTION, DIO_U8_PB5_INITIAL_DIRECTION, DIO_U8_PB4_INITIAL_DIRECTION, DIO_U8_PB3_INITIAL_DIRECTION, DIO_U8_PB2_INITIAL_DIRECTION, DIO_U8_PB1_INITIAL_DIRECTION, DIO_U8_PB0_INITIAL_DIRECTION );
	/* PORTC */
	DIO_U8_DDRC_REG = DIO_U8_CONC( DIO_U8_PC7_INITIAL_DIRECTION, DIO_U8_PC6_INITIAL_DIRECTION, DIO_U8_PC5_INITIAL_DIRECTION, DIO_U8_PC4_INITIAL_DIRECTION, DIO_U8_PC3_INITIAL_DIRECTION, DIO_U8_PC2_INITIAL_DIRECTION, DIO_U8_PC1_INITIAL_DIRECTION, DIO_U8_PC0_INITIAL_DIRECTION );
	/* PORTD */
	DIO_U8_DDRD_REG = DIO_U8_CONC( DIO_U8_PD7_INITIAL_DIRECTION, DIO_U8_PD6_INITIAL_DIRECTION, DIO_U8_PD5_INITIAL_DIRECTION, DIO_U8_PD4_INITIAL_DIRECTION, DIO_U8_PD3_INITIAL_DIRECTION, DIO_U8_PD2_INITIAL_DIRECTION, DIO_U8_PD1_INITIAL_DIRECTION, DIO_U8_PD0_INITIAL_DIRECTION );
	
	/* Set Initial Port Value */
	/* PORTA */
	DIO_U8_PORTA_REG = DIO_U8_CONC( DIO_U8_PA7_INITIAL_VALUE, DIO_U8_PA6_INITIAL_VALUE, DIO_U8_PA5_INITIAL_VALUE, DIO_U8_PA4_INITIAL_VALUE, DIO_U8_PA3_INITIAL_VALUE, DIO_U8_PA2_INITIAL_VALUE, DIO_U8_PA1_INITIAL_VALUE, DIO_U8_PA0_INITIAL_VALUE );
	/* PORTB */
	DIO_U8_PORTB_REG = DIO_U8_CONC( DIO_U8_PB7_INITIAL_VALUE, DIO_U8_PB6_INITIAL_VALUE, DIO_U8_PB5_INITIAL_VALUE, DIO_U8_PB4_INITIAL_VALUE, DIO_U8_PB3_INITIAL_VALUE, DIO_U8_PB2_INITIAL_VALUE, DIO_U8_PB1_INITIAL_VALUE, DIO_U8_PB0_INITIAL_VALUE );
	/* PORTC */
	DIO_U8_PORTC_REG = DIO_U8_CONC( DIO_U8_PC7_INITIAL_VALUE, DIO_U8_PC6_INITIAL_VALUE, DIO_U8_PC5_INITIAL_VALUE, DIO_U8_PC4_INITIAL_VALUE, DIO_U8_PC3_INITIAL_VALUE, DIO_U8_PC2_INITIAL_VALUE, DIO_U8_PC1_INITIAL_VALUE, DIO_U8_PC0_INITIAL_VALUE );
	/* PORTD */
	DIO_U8_PORTD_REG = DIO_U8_CONC( DIO_U8_PD7_INITIAL_VALUE, DIO_U8_PD6_INITIAL_VALUE, DIO_U8_PD5_INITIAL_VALUE, DIO_U8_PD4_INITIAL_VALUE, DIO_U8_PD3_INITIAL_VALUE, DIO_U8_PD2_INITIAL_VALUE, DIO_U8_PD1_INITIAL_VALUE, DIO_U8_PD0_INITIAL_VALUE );
}

/*
 Name: DIO_u8SetPinDirection
 Input:  u8 PortId, u8 PinId, and u8 PinDirection
 Output: u8 Error or No Error
 Description: Function to set Pin direction.
*/
u8 DIO_u8SetPinDirection ( u8 Cpy_u8PortId, u8 Cpy_u8PinId, u8 Cpy_u8PinDirection )
{
	/* Define local variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range */
	if ( ( Cpy_u8PortId <= DIO_U8_PORTD ) && ( Cpy_u8PinId <= DIO_U8_PIN7 ) )
	{
		/* Check 1.1: PinDirection is in the valid range */
		if ( Cpy_u8PinDirection == DIO_U8_PIN_INPUT )
		{
			/* Check 1.1.1: Required PortId */
			switch ( Cpy_u8PortId )
			{
				case DIO_U8_PORTA: CLR_BIT( DIO_U8_DDRA_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTB: CLR_BIT( DIO_U8_DDRB_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTC: CLR_BIT( DIO_U8_DDRC_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTD: CLR_BIT( DIO_U8_DDRD_REG, Cpy_u8PinId ); break;
			}
		}
		/* Check 1.2: PinDirection is in the valid range */
		else if ( Cpy_u8PinDirection == DIO_U8_PIN_OUTPUT )
		{
			/* Check 1.2.1: Required PortId */
			switch ( Cpy_u8PortId )
			{
				case DIO_U8_PORTA: SET_BIT( DIO_U8_DDRA_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTB: SET_BIT( DIO_U8_DDRB_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTC: SET_BIT( DIO_U8_DDRC_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTD: SET_BIT( DIO_U8_DDRD_REG, Cpy_u8PinId ); break;
			}
		}
		/* Check 1.3: PinDirection is not in the valid range */
		else
		{
			/* Update error state = NOK, wrong PinDirection! */
			Loc_u8ErrorState = STD_TYPES_NOK;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId or PinId! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}
	
	return Loc_u8ErrorState;
}

/*
 Name: DIO_u8SetPinValue
 Input:  u8 PortId, u8 PinId, and u8 PinValue
 Output: u8 Error or No Error
 Description: Function to set Pin value.
*/
u8 DIO_u8SetPinValue     ( u8 Cpy_u8PortId, u8 Cpy_u8PinId, u8 Cpy_u8PinValue )
{
	/* Define local variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range */
	if ( ( Cpy_u8PortId <= DIO_U8_PORTD ) && ( Cpy_u8PinId <= DIO_U8_PIN7 ) )
	{
		/* Check 1.1: PinValue is in the valid range */
		if ( Cpy_u8PinValue == DIO_U8_PIN_LOW )
		{
			/* Check 1.1.1: Required PortId */
			switch ( Cpy_u8PortId )
			{
				case DIO_U8_PORTA: CLR_BIT( DIO_U8_PORTA_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTB: CLR_BIT( DIO_U8_PORTB_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTC: CLR_BIT( DIO_U8_PORTC_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTD: CLR_BIT( DIO_U8_PORTD_REG, Cpy_u8PinId ); break;
			}
		}
		/* Check 1.2: PinValue is in the valid range */
		else if ( Cpy_u8PinValue == DIO_U8_PIN_HIGH )
		{
			/* Check 1.2.1: Required PortId */
			switch( Cpy_u8PortId )
			{
				case DIO_U8_PORTA: SET_BIT( DIO_U8_PORTA_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTB: SET_BIT( DIO_U8_PORTB_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTC: SET_BIT( DIO_U8_PORTC_REG, Cpy_u8PinId ); break;
				case DIO_U8_PORTD: SET_BIT( DIO_U8_PORTD_REG, Cpy_u8PinId ); break;
			}
		}
		/* Check 1.3: PinValue is not in the valid range */
		else
		{
			/* Update error state = NOK, wrong PinValue! */
			Loc_u8ErrorState = STD_TYPES_NOK;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId or PinId! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}
	
	return Loc_u8ErrorState;
}

/*
 Name: DIO_u8GetPinValue
 Input: u8 PortId, u8 PinId, and Pointer to u8 ReturnedPinValue
 Output: u8 Error or No Error
 Description: Function to get Pin value.
*/
u8 DIO_u8GetPinValue 	 ( u8 Cpy_u8PortId, u8 Cpy_u8PinId, u8 *Cpy_pu8ReturnedPinValue )
{
	/* Define local variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range, and Pointer is not equal to NULL */
	if ( ( Cpy_u8PortId <= DIO_U8_PORTD ) && ( Cpy_u8PinId <= DIO_U8_PIN7 ) && ( Cpy_pu8ReturnedPinValue != NULL ) )
	{
		/* Define local variable to put the value of PIN register in it, incase DIO_u8_PIN_LOW or DIO_u8_PIN_HIGH values are changed */
		u8 Loc_u8PinValue;
	
		/* Check 1.1: Required PortId */
		switch ( Cpy_u8PortId )
		{
			case DIO_U8_PORTA:
			
				Loc_u8PinValue = GET_BIT( DIO_U8_PINA_REG, Cpy_u8PinId );
				
				/* Check 1.1.1: PinValue is Low */
				if ( Loc_u8PinValue == 0 )
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_LOW;
				}
				/* Check 1.1.2: PinValue is High */
				else
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_HIGH;
				}					
			break;
			
			case DIO_U8_PORTB:
			
				Loc_u8PinValue = GET_BIT( DIO_U8_PINB_REG, Cpy_u8PinId );

				/* Check 1.1.3: PinValue is Low */
				if ( Loc_u8PinValue == 0 )
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_LOW;
				}
				/* Check 1.1.4: PinValue is High */
				else
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_HIGH;
				}					
			break;
				
			case DIO_U8_PORTC:

				Loc_u8PinValue = GET_BIT( DIO_U8_PINC_REG, Cpy_u8PinId );
				
				/* Check 1.1.5: PinValue is Low */
				if ( Loc_u8PinValue == 0 )
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_LOW;
				}
				/* Check 1.1.6: PinValue is High */
				else
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_HIGH;
				}	
			break;
								
			case DIO_U8_PORTD:

				Loc_u8PinValue = GET_BIT( DIO_U8_PIND_REG, Cpy_u8PinId );

				/* Check 1.1.7: PinValue is Low */
				if ( Loc_u8PinValue == 0 )
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_LOW;
				}
				/* Check 1.1.8: PinValue is High */
				else
				{
					*Cpy_pu8ReturnedPinValue = DIO_U8_PIN_HIGH;
				}	
			break;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong PortId, PinId, or Pointer is NULL! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}

	return Loc_u8ErrorState;
}

/*
 Name: DIO_u8TogglePinValue
 Input: u8 PortId and u8 PinId
 Output: u8 Error or No Error
 Description: Function to toggle Pin value.
*/
u8 DIO_u8TogglePinValue  ( u8 Cpy_u8PortId, u8 Cpy_u8PinId )
{
	/* Define local variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range */
	if ( ( Cpy_u8PortId <= DIO_U8_PORTD ) && ( Cpy_u8PinId <= DIO_U8_PIN7 ) )
	{
		/* Check 1.1: Required PortId */
		switch ( Cpy_u8PortId )
		{
			case DIO_U8_PORTA: TOG_BIT( DIO_U8_PORTA_REG, Cpy_u8PinId ); break;
			case DIO_U8_PORTB: TOG_BIT( DIO_U8_PORTB_REG, Cpy_u8PinId ); break;
			case DIO_U8_PORTC: TOG_BIT( DIO_U8_PORTC_REG, Cpy_u8PinId ); break;
			case DIO_U8_PORTD: TOG_BIT( DIO_U8_PORTD_REG, Cpy_u8PinId ); break;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId or PinId! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}
	
	return Loc_u8ErrorState;
}

/*
 Name: DIO_u8SetPortDirection
 Input: u8 PortId and u8 PortDirection
 Output: u8 Error or No Error
 Description: Function to set Port direction.
*/
u8 DIO_u8SetPortDirection( u8 Cpy_u8PortId, u8 Cpy_u8PortDirection )
{
	/* Define local variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;
	
	/* Check 1: PortId is in the valid range */
	if( Cpy_u8PortId <= DIO_U8_PORTD )
	{		
		/* Check 1.1: Required PortId */
		switch( Cpy_u8PortId )
		{
			case DIO_U8_PORTA: DIO_U8_DDRA_REG = Cpy_u8PortDirection; break;
			case DIO_U8_PORTB: DIO_U8_DDRB_REG = Cpy_u8PortDirection; break;
			case DIO_U8_PORTC: DIO_U8_DDRC_REG = Cpy_u8PortDirection; break;
			case DIO_U8_PORTD: DIO_U8_DDRD_REG = Cpy_u8PortDirection; break;
		}
	}
	/* Check 2: PortId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}
	
	return Loc_u8ErrorState;
}

/*
 Name: DIO_u8SetPortValue
 Input: u8 PortId and u8 PortValue
 Output: u8 Error or No Error
 Description: Function to set Port value.
*/
u8 DIO_u8SetPortValue	 ( u8 Cpy_u8PortId, u8 Cpy_u8PortValue )
{
	/* Define Local Variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;
	
	/* Check 1: PortId is in the valid range */
	if( Cpy_u8PortId <= DIO_U8_PORTD )
	{
		/* Check 1.1: Required PortId */
		switch( Cpy_u8PortId )
		{
			case DIO_U8_PORTA: DIO_U8_PORTA_REG = Cpy_u8PortValue; break;
			case DIO_U8_PORTB: DIO_U8_PORTB_REG = Cpy_u8PortValue; break;
			case DIO_U8_PORTC: DIO_U8_PORTC_REG = Cpy_u8PortValue; break;
			case DIO_U8_PORTD: DIO_U8_PORTD_REG = Cpy_u8PortValue; break;
		}
	}
	/* Check 2: PortId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}
	
	return Loc_u8ErrorState;
}

/*
 Name: DIO_u8GetPortValue
 Input: u8 PortId and Pointer to u8 ReturnedPortValue
 Output: u8 Error or No Error
 Description: Function to get Port value.
*/
u8 DIO_u8GetPortValue	 ( u8 Cpy_u8PortId, u8 *Cpy_pu8ReturnedPortValue )
{
	/* Define Local Variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;
	
	/* Check 1: PortId is in the valid range, and Pointer is not equal to NULL */
	if( ( Cpy_u8PortId <= DIO_U8_PORTD ) && ( Cpy_pu8ReturnedPortValue != NULL ) )
	{
		/* Check 1.1: Required PortId */
		switch( Cpy_u8PortId )
		{
			case DIO_U8_PORTA: *Cpy_pu8ReturnedPortValue = DIO_U8_PINA_REG; break;
			case DIO_U8_PORTB: *Cpy_pu8ReturnedPortValue = DIO_U8_PINB_REG; break;
			case DIO_U8_PORTC: *Cpy_pu8ReturnedPortValue = DIO_U8_PINC_REG; break;
			case DIO_U8_PORTD: *Cpy_pu8ReturnedPortValue = DIO_U8_PIND_REG; break;
		}		
	}
	/* Check 2: PortId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong PortId, or Pointer is NULL! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}
	
	return Loc_u8ErrorState;
}
