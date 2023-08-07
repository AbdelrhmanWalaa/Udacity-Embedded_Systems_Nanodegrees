/*
 * APP_Program.c
 *
 *  Created on: Sep 10, 2022
 *      Author: Abdelrhman Walaa
 * Description: This file contains all Traffic Light Control System (TLCS) Application functions' implementations. *
 */

/* 8 MegaHertz System Frequency */
#define F_CPU 8000000UL

/* APP */
#include "APP_Interface.h"
#include "APP_Config.h"

/* Declaration and Initialization */
/* Global variables to store Current and Next traffic state */
static u8 Glb_u8CurTrafficState = APP_U8_TRAFFIC_LIGHT_GREEN;
static u8 Glb_u8NxtTrafficState = APP_U8_TRAFFIC_LIGHT_YELLOW;

/* Global variable to store Mode state */
static u8 Glb_u8ModeState = APP_U8_CAR_MODE;

/* Global variable to store OVF Flag state */
static u8 Glb_u8OVFFlag = APP_U8_FLAG_DOWN;

/* Global variable to store Direction Flag state */
static u8 Glb_u8DirectionFlag = APP_U8_FLAG_UP;

/*
 Name: APP_vdStartProgram
 Input: void
 Output: void
 Description: Function to start program by calling all layers needed.
*/
vd APP_vdStartProgram      ( void )
{
	/* Configure SetCallBack Functions */
	/* External Interrupt 0 */
	EXI_u8SetCallBack( EXI_U8_INT0, &APP_vdSwitchMode );
	/* Timer/Counter 0 */
	TMR_u8OVFSetCallBack( TMR_U8_TMR0, &APP_vdCarMode );

	/* Initialize Peripherals: */
	/* Digital Input/Output */
	DIO_vdInitialization();
	/* Global Interrupt */
	GLI_vdEnableGIE();
	/* External Interrupt 0 */
	EXI_u8EnablePIE( EXI_U8_INT0, EXI_U8_SENSE_RISING_EDGE );
	/* Timer/Counter 0 */
	TMR_vdTMR0Initialization();

	/* Define local variable to get number of OVFs */
	u16 Loc_u16NumberOfOVF = 0;

	/* Loop: Forever */
	while(1)
	{
		/* Check 1: ModeState is PED_MODE */
		if ( Glb_u8ModeState == APP_U8_PED_MODE )
		{
			APP_vdPedestrianMode();
		}

		/* Check 2: CurrTrafficState is Yellow */
		if ( Glb_u8CurTrafficState == APP_U8_TRAFFIC_LIGHT_YELLOW )
		{
			/* Start Blinking */
			/* Get number of OVFs */
			TMR_u8GetNumberOfOVF( TMR_U8_TMR0, &Loc_u16NumberOfOVF );

			/* Check 2.1: Number of OVFs is divisible by 500, i.e.: Toggle/Blink Yellow LED every 500 OVFs */
			if ( Loc_u16NumberOfOVF % 500 == 0 )
			{
				DIO_u8TogglePinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED );
			}
		}
		/* Check 3: CurrTrafficState is not Yellow */
		else
		{
			DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED, DIO_U8_PIN_LOW );
		}
	}
}

/*
 Name: APP_vdSwitchMode
 Input: void
 Output: void
 Description: Function to switch from normal Car mode to Pedestrian mode, it will be called back when External Interrupt takes place.
*/
vd APP_vdSwitchMode		   ( void )
{
	/* Update mode state = PED_MODE */
	Glb_u8ModeState = APP_U8_PED_MODE;
}

/*
 Name: APP_vdCheckOVFFlag
 Input: void
 Output: void
 Description: Function to be called back when Timer Overflow Interrupt takes place, this function takes place in Pedestrian mode only.
*/
vd APP_vdSetOVFFlag	       ( void )
{
	/* Update OVF flag = UP */
	Glb_u8OVFFlag = APP_U8_FLAG_UP;
}

/*
 Name: APP_vdCarMode
 Input: void
 Output: void
 Description: Function to be called back when Timer Overflow Interrupt takes place.
*/
vd APP_vdCarMode	       ( void )
{
	/* Check 1: Required TrafficState */
    switch ( Glb_u8NxtTrafficState )
    {
        case APP_U8_TRAFFIC_LIGHT_GREEN:

        	/* Update CurrTrafficState to Green and NxtTrafficState to Yellow */
        	Glb_u8CurTrafficState = APP_U8_TRAFFIC_LIGHT_GREEN;
        	Glb_u8NxtTrafficState = APP_U8_TRAFFIC_LIGHT_YELLOW;

			/* Update flag state = UP */
        	Glb_u8DirectionFlag = APP_U8_FLAG_UP;

        	/* Turn on Green for Car */
        	APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_GREEN );
            break;

        case APP_U8_TRAFFIC_LIGHT_YELLOW:

        	/* Update CurrTrafficState to Yellow */
        	Glb_u8CurTrafficState = APP_U8_TRAFFIC_LIGHT_YELLOW;

        	/* Update NxtTrafficState to Green if Flag state is DOWN, or Red if Flag state is UP */
        	/* Check 1.1: Required FlagState */
            switch ( Glb_u8DirectionFlag )
            {
            	case APP_U8_FLAG_DOWN: Glb_u8NxtTrafficState = APP_U8_TRAFFIC_LIGHT_GREEN; break;
            	case APP_U8_FLAG_UP  : Glb_u8NxtTrafficState = APP_U8_TRAFFIC_LIGHT_RED;   break;
            }

            /* Turn on Yellow for Car */
            APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_YELLOW );
            break;

        case APP_U8_TRAFFIC_LIGHT_RED:

        	/* Update CrrentTrafficState to Red and NxtTrafficState to Yellow */
        	Glb_u8CurTrafficState = APP_U8_TRAFFIC_LIGHT_RED;
        	Glb_u8NxtTrafficState = APP_U8_TRAFFIC_LIGHT_YELLOW;

            /* Update flag state = DOWN */
        	Glb_u8DirectionFlag = APP_U8_FLAG_DOWN;

        	/* Turn on Red for Car */
        	APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_RED );
            break;
    }
}

/*
 Name: APP_vdPedestrianMode
 Input: void
 Output: void
 Description: Function to to be called if External Interrupt takes place.
*/
vd APP_vdPedestrianMode    ( void )
{
	/* Step 1: Reset Timer */
	/* Disable Timer/Counter0 Counting */
	TMR_u8DisableTMR( TMR_U8_TMR0 );
	/* Change OVF Callback function to APP_vdSetOVFFlag() */
	TMR_u8OVFSetCallBack( TMR_U8_TMR0, &APP_vdSetOVFFlag );
	/* Enable Timer/Counter0 Counting */
	TMR_u8EnableTMR ( TMR_U8_TMR0 );

	/* Define local variable to get number of OVFs */
	u16 Loc_u16NumberOfOVF = 0;

	/* Step 2: Run the traffic light sequence according to the CurrTrafficState */
	/* Check 1: Required CurrTrafficState */
	switch ( Glb_u8CurTrafficState )
	{
		case APP_U8_TRAFFIC_LIGHT_GREEN :
		case APP_U8_TRAFFIC_LIGHT_YELLOW:

			/* Step A: Turn on Yellow for Car and Pedestrian */
			APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_YELLOW );
			APP_u8SetPEDTrafficLight( APP_U8_TRAFFIC_LIGHT_YELLOW );

			/* Loop: Until OVFFlag is UP, i.e.: Poll for 5 sec. */
			while ( Glb_u8OVFFlag == APP_U8_FLAG_DOWN )
			{
				/* Start Blinking */
				/* Get number of OVFs */
				TMR_u8GetNumberOfOVF( TMR_U8_TMR0, &Loc_u16NumberOfOVF );

				DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_RED_LED, DIO_U8_PIN_LOW );

				/* Check 1.1: Number of OVFs is divisible by 500, i.e.: Toggle/Blink Yellow LED every 500 OVFs */
				if ( Loc_u16NumberOfOVF % 500 == 0 )
				{
					DIO_u8TogglePinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED );
					DIO_u8TogglePinValue( APP_U8_PED_LED_PORT, APP_U8_PED_YELLOW_LED );
				}
			}

			/* Update OVF flag = DOWN */
			Glb_u8OVFFlag = APP_U8_FLAG_DOWN;

			/* Step B: Turn on Red for Car, and Green for Pedestrian */
			APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_RED );
			APP_u8SetPEDTrafficLight( APP_U8_TRAFFIC_LIGHT_GREEN );

			/* Loop: Until OVFFlag is UP, i.e.: Poll for 5 sec. */
			while ( Glb_u8OVFFlag == APP_U8_FLAG_DOWN );

			/* Update OVF flag = DOWN */
			Glb_u8OVFFlag = APP_U8_FLAG_DOWN;

			/* Step C: Turn on Yellow for Car and Pedestrian */
			APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_YELLOW );
			APP_u8SetPEDTrafficLight( APP_U8_TRAFFIC_LIGHT_YELLOW );

			/* Loop: Until OVFFlag is UP, i.e.: Poll for 5 sec. */
			while ( Glb_u8OVFFlag == APP_U8_FLAG_DOWN )
			{
				/* Start Blinking */
				/* Get number of OVFs */
				TMR_u8GetNumberOfOVF( TMR_U8_TMR0, &Loc_u16NumberOfOVF );

				DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_RED_LED, DIO_U8_PIN_LOW );

				/* Check 1.2: Number of OVFs is divisible by 500, i.e.: Toggle/Blink Yellow LED every 500 OVFs */
				if ( Loc_u16NumberOfOVF % 500 == 0 )
				{
					DIO_u8TogglePinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED );
					DIO_u8TogglePinValue( APP_U8_PED_LED_PORT, APP_U8_PED_YELLOW_LED );
				}
			}

			/* Update OVF flag = DOWN */
			Glb_u8OVFFlag = APP_U8_FLAG_DOWN;

			/* Step D: Turn on Green for Car, and Red Pedestrian */
			APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_GREEN );
			APP_u8SetPEDTrafficLight( APP_U8_TRAFFIC_LIGHT_RED );

			break;

		case APP_U8_TRAFFIC_LIGHT_RED   :

			/* Step A: Turn on Red for Car, and Green for Pedestrian */
			APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_RED );
			APP_u8SetPEDTrafficLight( APP_U8_TRAFFIC_LIGHT_GREEN );

			/* Loop: Until OVFFlag is UP, i.e.: Poll for 5 sec. */
			while ( Glb_u8OVFFlag == APP_U8_FLAG_DOWN );

			/* Update OVF flag = DOWN */
			Glb_u8OVFFlag = APP_U8_FLAG_DOWN;

			/* Step B: Turn on Yellow for Car and Pedestrian */
			APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_YELLOW );
			APP_u8SetPEDTrafficLight( APP_U8_TRAFFIC_LIGHT_YELLOW );

			/* Loop: Until OVFFlag is UP, i.e.: Poll for 5 sec. */
			while ( Glb_u8OVFFlag == APP_U8_FLAG_DOWN )
			{
				/* Start Blinking */
				/* Get number of OVFs */
				TMR_u8GetNumberOfOVF( TMR_U8_TMR0, &Loc_u16NumberOfOVF );

				DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_RED_LED, DIO_U8_PIN_LOW );

				/* Check 1.3: Number of OVFs is divisible by 500, i.e.: Toggle/Blink Yellow LED every 500 OVFs */
				if ( Loc_u16NumberOfOVF % 500 == 0 )
				{
					DIO_u8TogglePinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED );
					DIO_u8TogglePinValue( APP_U8_PED_LED_PORT, APP_U8_PED_YELLOW_LED );
				}
			}

			/* Update OVF flag = DOWN */
			Glb_u8OVFFlag = APP_U8_FLAG_DOWN;

			/* Step C: Turn on Green for Car, and Red for Pedestrian */
			APP_u8SetCARTrafficLight( APP_U8_TRAFFIC_LIGHT_GREEN );
			APP_u8SetPEDTrafficLight( APP_U8_TRAFFIC_LIGHT_RED );

			break;
	}

	/* Step 3: Switch to Car Mode */
	/* Update mode state = CAR_MODE */
	Glb_u8ModeState = APP_U8_CAR_MODE;
	/* Update current traffic state = GREEN, and next traffic state = YELLOW */
	Glb_u8CurTrafficState = APP_U8_TRAFFIC_LIGHT_GREEN;
	Glb_u8NxtTrafficState = APP_U8_TRAFFIC_LIGHT_YELLOW;
	/* Update direction flag = UP */
	Glb_u8DirectionFlag = APP_U8_FLAG_UP;

	/* Step 4: Reset Timer */
	/* Disable Timer/Counter0 Counting */
	TMR_u8DisableTMR( TMR_U8_TMR0 );
	/* Change OVF Callback function to APP_vdCarMode() */
	TMR_u8OVFSetCallBack( TMR_U8_TMR0, &APP_vdCarMode );
	/* Enable Timer/Counter0 Counting */
	TMR_u8EnableTMR ( TMR_U8_TMR0 );
}

/*
 Name: APP_u8SetCARTrafficLight
 Input: u8 TrafficLight
 Output: u8 Error or No Error
 Description: Function to set traffic light to Cars.
*/
u8 APP_u8SetCARTrafficLight( u8 Cpy_u8TrafficLight )
{
	/* Define local variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;

	/* Check 1: TrafficLight is in the valid range */
	if ( Cpy_u8TrafficLight <= APP_U8_TRAFFIC_LIGHT_RED )
	{
		/* Check 1.1: Required TrafficLight */
			switch ( Cpy_u8TrafficLight )
			{
				case APP_U8_TRAFFIC_LIGHT_GREEN:
		            DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_GREEN_LED, DIO_U8_PIN_HIGH );
					DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED, DIO_U8_PIN_LOW );
					DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_RED_LED, DIO_U8_PIN_LOW );
					break;

				case APP_U8_TRAFFIC_LIGHT_YELLOW:
		            DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_GREEN_LED, DIO_U8_PIN_LOW );
		            DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED, DIO_U8_PIN_HIGH );
		            DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_RED_LED, DIO_U8_PIN_LOW );
		            break;

				case APP_U8_TRAFFIC_LIGHT_RED:
		            DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_GREEN_LED, DIO_U8_PIN_LOW );
		            DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_YELLOW_LED, DIO_U8_PIN_LOW );
		            DIO_u8SetPinValue( APP_U8_CAR_LED_PORT, APP_U8_CAR_RED_LED, DIO_U8_PIN_HIGH );
		            break;
			}
	}
	/* Check 2: TrafficLight is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong Traffic Light! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}

	return Loc_u8ErrorState;
}

/*
 Name: APP_u8SetPEDTrafficLight
 Input: u8 TrafficLight
 Output: u8 Error or No Error
 Description: Function to set traffic light to Pedestrians.
*/
u8 APP_u8SetPEDTrafficLight( u8 Cpy_u8TrafficLight )
{
	/* Define local variable to set the error state = OK */
	u8 Loc_u8ErrorState = STD_TYPES_OK;

	/* Check 1: TrafficLight is in the valid range */
	if ( Cpy_u8TrafficLight <= APP_U8_TRAFFIC_LIGHT_RED )
	{
		/* Check 1.1: RequiredTraffic Light */
			switch ( Cpy_u8TrafficLight )
			{
				case APP_U8_TRAFFIC_LIGHT_GREEN:
		            DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_GREEN_LED, DIO_U8_PIN_HIGH );
					DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_YELLOW_LED, DIO_U8_PIN_LOW );
					DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_RED_LED, DIO_U8_PIN_LOW );
					break;

				case APP_U8_TRAFFIC_LIGHT_YELLOW:
		            DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_GREEN_LED, DIO_U8_PIN_LOW );
		            DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_YELLOW_LED, DIO_U8_PIN_HIGH );
		            DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_RED_LED, DIO_U8_PIN_LOW );
		            break;

				case APP_U8_TRAFFIC_LIGHT_RED:
		            DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_GREEN_LED, DIO_U8_PIN_LOW );
		            DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_YELLOW_LED, DIO_U8_PIN_LOW );
		            DIO_u8SetPinValue( APP_U8_PED_LED_PORT, APP_U8_PED_RED_LED, DIO_U8_PIN_HIGH );
		            break;
			}
	}
	/* Check 2: TrafficLight is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong Traffic Light! */
		Loc_u8ErrorState = STD_TYPES_NOK;
	}

	return Loc_u8ErrorState;
}
