/* Standard Library */
#include <stdio.h>
#include <string.h>

/* Library Module */
#include "../Library/standard_types.h"

/* Console Module */
#include "../Console/console.h"

/* Card Module */
#include "../Card/card.h"
/* Terminal Module */
#include "../Terminal/terminal.h"
/* Server Module */
#include "../Server/server.h"

/* Application Module */
#include "app.h"

extern ST_accountsDB_t accountsDB[255];
extern uint8_t Glb_AccountsDBIndex;

/*
 Name: appStart
 Input: void
 Output: void
 Description: This function will be called by main to start the program.
*/
void appStart(void)
{
    /* Variables to be used */
    ST_cardData_t     cardData;
    ST_terminalData_t terminalData;
    ST_transaction_t  currentTransaction;

    uint8_t Loc_UserInput;

    /* Set Terminal max Amount */
    setMaxAmount(&terminalData);

    /* Start of program */

    /* Print out message: Starting the program */
    systemPrintOut(" Starting the program....");
    /* Print out message: Welcome */
    systemPrintOut("\t\tWelcome!");

    while (1)
    {   
        /* Initialize Arrays with NULL character */
        memset(cardData.cardHolderName, '\0', 25);
        memset(cardData.primaryAccountNumber, '\0', 20);
        memset(cardData.cardExpirationDate, '\0', 6);
        memset(terminalData.transactionDate, '\0', 11);

        /* Clear Buffer */
        fflush(stdin);

        printf("\n\n");
        printf("\t\t  -VBS-\n\t Virtual Banking System");
        printf("\n\n");

        /* Get Card Data */
        printf("\n\n");
        printf(" Insert Card Data");
        printf("\n\n");

        /* Loop: Until user enters correct Name */
        while (getCardHolderName(&cardData) == WRONG_NAME)
        {
            systemDeleteLine(" Error! Wrong Name!");
        }
        /* Loop: Until user enters correct PAN */
        while (getCardPAN(&cardData) == WRONG_PAN)
        {
            systemDeleteLine(" Error! Wrong PAN!");
        }
        /* Loop: Until user enters correct Expiration Date */
        while (getCardExpiryDate(&cardData) == WRONG_EXP_DATE)
        {
            systemDeleteLine(" Error! Wrong Expiration Date!");
        }

        /* Print out message: Processing */
        systemPrintOut(" Processing....");

        printf("\n\n");
        printf("\t\t  -VBS-\n\t Virtual Banking System");
        printf("\n\n");

        /* Get Terminal Data */
        printf("\n\n");
        printf(" Insert Terminal Data");
        printf("\n\n");

        /* Loop: Until user enters correct Transaction Date */
        while (getTransactionDate(&terminalData) == WRONG_DATE)
        {
            systemDeleteLine(" Error! Wrong Transaction Date!");
        }

        /* Check 1: Card is Expired */
        if (isCardExpired(&cardData, &terminalData) == EXPIRED_CARD)
        {
            /* Print out error: Expired Card */
            systemPrintOut(" Error! Expired card!");
        }
        /* Check 2: Card is not Expired */
        else
        {
            /* Loop: Until user enters valid Amount */
            while (getTransactionAmount(&terminalData) == INVALID_AMOUNT)
            {
                systemDeleteLine(" Error! Wrong Amount!");
            }

            /* Check 2.1: User enters Amount exceeds Terminal Max Amount */
            if (isBelowMaxAmount(&terminalData) == EXCEED_MAX_AMOUNT)
            {
                /* Print out error: Exceed Max Amount */
                systemPrintOut(" Error! Exceed Max Amount!");
            }
            /* Check 2.2: User enters Amount does not exceed Max Amount */
            else
            {
                /* Print out message: Processing */
                systemPrintOut(" Processing....");

                /* Save the current card data in the current Transaction structure */
                currentTransaction.cardHolderData = cardData;
                /* Save the current terminal data in the current Transaction structure */
                currentTransaction.terminalData = terminalData;

                uint8_t currentState = recieveTransactionData(&currentTransaction);

                /* Check 2.2.1: Current state of Transaction  */
                switch (currentState)
                {
                    case FRAUD_CARD:
                        /* Print out message: Declined Invalid Account */
                        systemPrintOut(" Declined Invalid Account!");
                        break;
                    case DECLINED_STOLEN_CARD:
                        /* Print out message: Declined Blocked Account */
                        systemPrintOut(" Declined Blocked Account!");
                        break;
                    case DECLINED_INSUFFECIENT_FUND:
                        /* Print out message: Declined Insufficient Funds */
                        systemPrintOut(" Declined Insufficient Funds!");
                        break;
                    case INTERNAL_SERVER_ERROR:
                        /* Print out message: Internal Server Error */
                        systemPrintOut(" Internal Server Error!");
                        break;
                    case APPROVED:
                        /* Print out message: Approved */
                        systemPrintOut(" Approved!");
                        printf("\n Your balance is %.2f \n", accountsDB[Glb_AccountsDBIndex].balance);
                        break;
                }                
            }
        }

        /* Clear Buffer */
        fflush(stdin);

        /* Confirm continue or exit */
        printf("\n\n");
        printf(" Press ANY KEY to continue or X to exit....  ");
        scanf("%c", &Loc_UserInput);

        /* Check 3: User pressed X */
        if (Loc_UserInput == 'X' || Loc_UserInput == 'x')
        {
            /* break while(1) */
            break;
        }

        /* Print out message: Processing */
        systemPrintOut(" Processing....");
    }

    /* Print out message: Goodbye */
    systemPrintOut("\t\tGoodbye!");
    /* Print out message: Exiting the program */
    systemPrintOut(" Exiting the program....");

    /* End of program */
}