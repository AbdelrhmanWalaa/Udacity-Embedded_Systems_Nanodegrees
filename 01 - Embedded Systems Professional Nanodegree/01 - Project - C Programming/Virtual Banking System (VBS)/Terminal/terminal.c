/* Standard Library */
#include <stdio.h>
#include <time.h>

/* Card Module */
#include "../Card/card.h"
/* Terminal Module */
#include "terminal.h"

/*
 Name: getTransactionDate
 Input: Pointer to Terminal Data structure
 Output: EN_terminalError_t Error or No Error
 Description: 1. This function will ask for the transaction data and store it in terminal data.
			  2. Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
			  3. If the transaction date is NULL, less than 10 characters or wrong format will return WRONG_DATE error,
				 else return TERMINAL_OK.
*/
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
	/* Define local variable to set the error state, No Error */
	EN_terminalError_t Loc_ErrorState = TERMINAL_OK;
	
	time_t t = time(NULL);
    struct tm tm = *localtime(&t);

	/* Get Date from system */
	sprintf(termData->transactionDate, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

	/* Get Transaction Date */
	printf(" Transaction Date: %s\n", termData->transactionDate);

	/* Check: No transactionDate or transactionDate < 10 or transactionDate > 10 or transactionDate is not in the correct format (DD/MM/YYYY) */
	if (termData->transactionDate[0] == '\0' || termData->transactionDate[9] == '\0' || termData->transactionDate[10] != '\0' || termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/')
	{
		/* Update error state, Wrong Date! */
		Loc_ErrorState = WRONG_DATE;
	}

	return Loc_ErrorState;
}

/*
 Name: isCardExpired
 Input: Card Data structure, Terminal Data structure
 Output: EN_terminalError_t Error or No Error
 Description: 1. This function compares the card expiry date with the transaction date.
			  2. If the card expiration date is before the transaction date will return EXPIRED_CARD,
				 else return TERMINAL_OK.
*/
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
	/* Define local variable to set the error state, No Error */
	EN_terminalError_t Loc_ErrorState = TERMINAL_OK;
	/* Define local variable to store month on Card */
	uint8_t Loc_CardMonth = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');
	/* Define local variable to store year on Card */
	uint8_t Loc_CardYear  = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');
	/* Define local variable to store month of transaction */
	uint8_t Loc_TransMonth = ((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0');
	/* Define local variable to store year of transaction */
	uint8_t Loc_TransYear  = ((termData->transactionDate[8] - '0') * 10) + (termData->transactionDate[9] - '0');

	/* Check 1: cardYear < transYear */
	if (Loc_CardYear < Loc_TransYear)
	{
		/* Update error state, Expired Card! */
		Loc_ErrorState = EXPIRED_CARD;
	}
	/* Check 2: cardYear >= transYear */
	else
	{
		/* Check 2.1: cardMonth < TransMonth */
		if (Loc_CardYear == Loc_TransYear && Loc_CardMonth < Loc_TransMonth)
		{
			/* Update error state, Expired Card! */
			Loc_ErrorState = EXPIRED_CARD;
		}
	}

	return Loc_ErrorState;
}

/*
 Name: isValidCardPAN
 Input: Pointer to Card Data structure
 Output: EN_terminalError_t Error or No Error
 Description: 1. This function checks if the PAN is a Luhn number or not.
			  2. If PAN is not a Luhn number will return INVALID_CARD, else return TERMINAL_OK.
*/
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
	/* Define local variable to set the error state, No Error */
	EN_terminalError_t Loc_ErrorState = TERMINAL_OK;
	/* Define local variables to check wether Number is Luhn or not  */
	uint8_t Loc_Counter = 0, Loc_Sum = 0, Loc_Number;
	uint8_t Loc_Index, Loc_IndexA, Loc_IndexB;
	uint8_t Loc_reversedPAN[20];
	
	/* Loop: Until the end of string */
	while(cardData->primaryAccountNumber[Loc_Counter] != '\0')
	{
		/* Increment Counter */
		Loc_Counter++;		
	}

	Loc_IndexA = Loc_Counter - 1;

	/* Loop: Until the PAN is reversed */
	for(Loc_IndexB = 0; Loc_IndexB < Loc_Counter; Loc_IndexB++, Loc_IndexA--)
	{
		Loc_reversedPAN[Loc_IndexB] = cardData->primaryAccountNumber[Loc_IndexA];		
	}

	/* Loop: Until the end of reversed PAN */
	for(Loc_Index = 0; Loc_Index < Loc_Counter; Loc_Index++)
	{
		Loc_Number = Loc_reversedPAN[Loc_Index] - '0';
		
		/* Check 1: Index is divisible by 2 */
		if(Loc_Index % 2 == 0)
		{
			/* Increment Sum */
			Loc_Sum += Loc_Number;
		}
		/* Check 2: Index is not divisible by 2 */
		else
		{
			/* Multiply Number by 2 */
			Loc_Number *= 2;

			/* Check 2.1: Number > 9 */
			if(Loc_Number > 9)
			{
				/* Add two digits of the Number */			
				Loc_Number = (Loc_Number / 10) + (Loc_Number % 10);					
			}

			/* Increment Sum */
			Loc_Sum += Loc_Number;
		}
	}

	/* Check 3: Sum is not divisible by 10 */
	if(Loc_Sum % 10 != 0)
	{
		/* Update error state, Invalid Card! */
		Loc_ErrorState = INVALID_CARD;
	}

	return Loc_ErrorState;
}

/*
 Name: getTransactionAmount
 Input: Pointer to Terminal Data structure
 Output: EN_terminalError_t Error or No Error
 Description: 1. This function asks for the transaction amount and saves it into terminal data.
			  2. If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else
				 return TERMINAL_OK.
*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	/* Define local variable to set the error state, No Error */
	EN_terminalError_t Loc_ErrorState = TERMINAL_OK;

	/* Get Transaction Amount */
	printf(" Amount:           ");
	scanf("%f", &termData->transAmount);

	/* Check: transAmount <= 0 */
	if (termData->transAmount <= 0)
	{
		/* Update error state, Invalid Amount! */
		Loc_ErrorState = INVALID_AMOUNT;
	}

	return Loc_ErrorState;
}

/*
 Name: isBelowMaxAmount
 Input: Pointer to Terminal Data structure
 Output: EN_terminalError_t Error or No Error
 Description: 1. This function compares the transaction amount with the terminal max amount.
			  2. If the transaction amount is larger than the terminal max amount will return
				 EXCEED_MAX_AMOUNT, else return TERMINAL_OK.
*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
	/* Define local variable to set the error state, No Error */
	EN_terminalError_t Loc_ErrorState = TERMINAL_OK;

	/* Check: transAmount > maxTransAmount */
	if (termData->transAmount > termData->maxTransAmount)
	{
		/* Update error state, Exceed Max Amount! */
		Loc_ErrorState = EXCEED_MAX_AMOUNT;
	}

	return Loc_ErrorState;
}

/*
 Name: setMaxAmount
 Input: Pointer to Terminal Data structure
 Output: EN_terminalError_t Error or No Error
 Description: 1. This function sets the maximum allowed amount into terminal data.
			  2. Transaction max amount is a float number.
			  3. If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error,
				 else return TERMINAL_OK.
*/
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
	/* Define local variable to set the error state, No Error */
	EN_terminalError_t Loc_ErrorState = TERMINAL_OK;

	termData->maxTransAmount = TERMINAL_MAX_AMOUNT;

	/* Check: maxTransAmount <= 0 */
	if (termData->maxTransAmount <= 0)
	{
		/* Update error state, Invalid Max Amount! */
		Loc_ErrorState = INVALID_MAX_AMOUNT;
	}

	return Loc_ErrorState;
}