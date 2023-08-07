/* Standard Library */
#include <stdio.h>

/* Card Module */
#include "card.h"
/* Terminal Module */
#include "../Terminal/terminal.h"

/*
 Name: getCardHolderName
 Input: Pointer to Card Data structure
 Output: EN_cardError_t Error or No Error
 Description: 1. This function will ask for the cardholder's name and store it into card data.
			  2. Card holder name is 24 alphabetic characters string max and 20 min.
			  3. if the cardholder name is NULL, less than 20 characters or more than 24 will return WRONG_NAME error,
				 else return CARD_OK.
*/
EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	/* Define local variable to set the error state, No Error */
	EN_cardError_t Loc_ErrorState = CARD_OK;

	/* Get Name */
	printf(" Name:        ");
	gets(cardData->cardHolderName);

	/* Check: No cardHolderName or cardHolderName < 20 or cardHolderName > 25  */
	if (cardData->cardHolderName[0] == '\0' || cardData->cardHolderName[19] == '\0' || cardData->cardHolderName[24] != '\0')
	{
		/* Update error state, Wrong Name! */
		Loc_ErrorState = WRONG_NAME;
	}

	return Loc_ErrorState;
}

/*
 Name: getCardExpiryDate
 Input: Pointer to Card Data structure
 Output: EN_cardError_t Error or No Error
 Description: 1. This function will ask for the card expiry date and store it in card data.
			  2. Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
			  3. If the card expiry date is NULL, less or more than 5 characters, or has the wrong format will return
				 WRONG_EXP_DATE error, else return CARD_OK.
*/
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
	/* Define local variable to set the error state, No Error */
	EN_cardError_t Loc_ErrorState = CARD_OK;

	/* Get Expiration Date */
	printf(" Expiry Date: ");
	gets(cardData->cardExpirationDate);

	/* Check: No cardExpirationDate or cardExpirationDate < 5 or cardExpirationDate > 5 or cardExpirationDate is not in the correct format (MM/YY) */
	if (cardData->cardExpirationDate[0] == '\0' || cardData->cardExpirationDate[4] == '\0' || cardData->cardExpirationDate[5] != '\0' || cardData->cardExpirationDate[2] != '/')
	{
		/* Update error state, Wrong Date! */
		Loc_ErrorState = WRONG_EXP_DATE;
	}

	return Loc_ErrorState;
}

/*
 Name: getCardPAN
 Input: Pointer to Card Data structure
 Output: EN_cardError_t Error or No Error
 Description: 1. This function will ask for the card's Primary Account Number and store it in card data.
			  2. PAN is 20 numeric characters string, 19 character max, and 16 character min.
			  3. If the PAN is NULL, less than 16 or more than 19 characters, will return WRONG_PAN error, else return CARD_OK.
*/
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
	/* Define local variable to set the error state, No Error */
	EN_cardError_t Loc_ErrorState = CARD_OK;

	/* Get Primary Account Number */
	printf(" PAN:         ");
	gets(cardData->primaryAccountNumber);

	/* Check: No cardPAN or cardPAN < 16 or cardPAN > 19 or cardPAN is not a Luhn number */
	if (cardData->primaryAccountNumber[0] == '\0' || cardData->primaryAccountNumber[15] == '\0' || cardData->primaryAccountNumber[19] != '\0' || isValidCardPAN(cardData) == INVALID_CARD)
	{
		/* Update error state, Wrong PAN! */
		Loc_ErrorState = WRONG_PAN;
	}

	return Loc_ErrorState;
}