/* Standard Library */
#include <string.h>

/* Card Module */
#include "../Card/card.h"
/* Terminal Module */
#include "../Terminal/terminal.h"
/* Server Module */
#include "server.h"

/* Accounts Database */
ST_accountsDB_t  accountsDB[255] =               /* Visa */                               /* MasterCard */
                                    /* Balance |  State |        PAN       */  /* Balance |  State |        PAN       */
                                   {{  12000   , BLOCKED, "4728459258966333"}, {  68600.3 , RUNNING, "5183150660610263"},
                                    {  5805.5  , RUNNING, "4946084897338284"}, {  5000.3  , RUNNING, "5400829062340903"},
                                    {  90360.12, RUNNING, "4728451059691228"}, {  1800000 , RUNNING, "5191786640828580"},
                                    {  16800.58, RUNNING, "4573762093153876"}, {  40800   , RUNNING, "5367052744350494"},
                                    {  520.9   , RUNNING, "4127856791257426"}, {  18900.45, RUNNING, "5248692364161088"},
                                    {  6900.33 , RUNNING, "4946099660091878"}, {  1047751 , RUNNING, "5419558003040483"},
                                    {  200000  , RUNNING, "4834699064563433"}, {  3026239 , RUNNING, "5116136307216426"},
                                    {  5000000 , RUNNING, "4946069587908256"}, {  9362076 , RUNNING, "5335847432506029"},
                                    {  25600   , RUNNING, "4946085117749481"}, {  10662670, RUNNING, "5424438206113309"},
                                    {  895000  , RUNNING, "4946099683908835"}, {  1824    , RUNNING, "5264166325336492"}};
/* Accounts Database Index */
uint8_t Glb_AccountsDBIndex = 0;

/* Transactions Database */
ST_transaction_t transactionsDB[255] = {0};
/* Transactions Database Index */
static uint8_t Glb_TransactionsDBIndex = 0;

/* 
 Name: recieveTransactionData
 Input: Pointer to Transaction structure
 Output: EN_transState_t Transaction State
 Description: 1. This function will take all transaction data and validate its data.
              2. It checks the account details and amount availability.
              3. If the account does not exist return FRAUD_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND, 
                 if the account is blocked will return DECLINED_STOLEN_CARD, if a transaction can't be saved will 
                 return INTERNAL_SERVER_ERROR and will not save the transaction, else returns APPROVED.
              4. It will update the database with the new balance.
*/
EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    /* Define local variable to set the transaction state, Approved */
    EN_transState_t Loc_TransState = APPROVED;
    /* Declare local variable to get all current account data from accountsDB */
    ST_accountsDB_t Loc_CurrentAccount;

    /* Check 1: Account is not found */
    if (isValidAccount(&transData->cardHolderData, &Loc_CurrentAccount) == ACCOUNT_NOT_FOUND)
    {
        /* Save the current Transaction state in the current transaction structure */
        transData->transState = FRAUD_CARD;

        /* Update transaction state, Fraud Card! */
        Loc_TransState = FRAUD_CARD;
    }
    /* Check 2: Account is found */
    else
    {
        /* Check 2.1: Account is blocked */
        if (isBlockedAccount(&Loc_CurrentAccount) == BLOCKED_ACCOUNT)
        {
            /* Save the current Transaction state in the current transaction structure */
            transData->transState = DECLINED_STOLEN_CARD;
            
            /* Update transaction state, Stolen Card! */
            Loc_TransState = DECLINED_STOLEN_CARD;          
        }
        /* Check 2.2: Amount is not available */
        else if (isAmountAvailable(&transData->terminalData, &Loc_CurrentAccount) == LOW_BALANCE)
        {
            /* Save the current Transaction state in the current transaction structure */
            transData->transState = DECLINED_INSUFFECIENT_FUND;

            /* Update transaction state, Stolen Card! */
            Loc_TransState = DECLINED_INSUFFECIENT_FUND;            
        }

        /* Check 2.3: Saving failed */
        if (saveTransaction(transData) == SAVING_FAILED)
        {
            /* Save the current Transaction state in the current transaction structure */
            transData->transState = INTERNAL_SERVER_ERROR;

            /* Update transaction state, Server Error! */
            Loc_TransState = INTERNAL_SERVER_ERROR;
        }
        /* Check 2.4: Saving succeed */
        else
        {
            /* Check 2.4.1: Account is not blocked and Amount is available */
            if (Loc_TransState != DECLINED_STOLEN_CARD && Loc_TransState != DECLINED_INSUFFECIENT_FUND)
            {
                /* Update Account in accountsDB with new balance after transaction approval */
                accountsDB[Glb_AccountsDBIndex].balance -= transData->terminalData.transAmount;
                
                /* Save the current Transaction state in the current transaction structure */
                transData->transState = APPROVED;
            }     
        }
    }

    return Loc_TransState;
}

/*
 Name: isValidAccount
 Input: Pointer to Card Data structure, 
 Output: EN_sreverError_t Error or No Error
 Description: 1. This function will take card data and validate if the account related to this card exists or not.
              2. It checks if the PAN exists or not in the server's database (searches for the card PAN in the DB).
              3. If the PAN doesn't exist will return ACCOUNT_NOT_FOUND, else will return SERVER_OK and return a reference 
                 to this account in the DB.
*/
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
    /* Define local variable to set the error state, No Error */
    EN_serverError_t Loc_ErrorState = SERVER_OK;
    /* Define local variable to set the flag state, Flag Down */
    EN_flagState_t Loc_Flag = FLAG_DOWN;
    
    /* Loop: Until Account is found or until the end of accountsDB */
    for (uint8_t Loc_Index = 0; Loc_Index < 255; Loc_Index++)
    {
        /* Check 1: Account is found */
        if (!strcmp(cardData->primaryAccountNumber, accountsDB[Loc_Index].primaryAccountNumber))
        {
            /* Copy Account details from accountsDB to passed pointer */
            *accountRefrence = accountsDB[Loc_Index];
            /* Update accountsDB Index */
            Glb_AccountsDBIndex = Loc_Index;

            /* Update flag state, Account is Found! */
            Loc_Flag = FLAG_UP;            
            break;
        }
    }

    /* Check 2: Account is not found */
    if (Loc_Flag == FLAG_DOWN)
    {
        /* Update error state, Account Not Found! */
        Loc_ErrorState = ACCOUNT_NOT_FOUND;
    }    

    return Loc_ErrorState;
}

/*
 Name: isBlockedAccount
 Input: 
 Output: EN_sreverError_t Error or No Error
 Description: 1. This function will take a reference to an existing account in the database.
              2. It checks if the account is blocked or not.
              3. If the account is blocked, will return BLOCKED_ACCOUNT, else will return SERVER_OK.
*/
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
    /* Define local variable to set the error state, No Error */
    EN_serverError_t Loc_ErrorState = SERVER_OK;

    /* Check: Account is Blocked */
    if (accountRefrence->state == BLOCKED)
    {
        /* Update error state, Blocked Account! */
        Loc_ErrorState = BLOCKED_ACCOUNT;
    }

    return Loc_ErrorState;
}

/*
 Name: isAmountAvailable
 Input: Pointer to Terminal Data structure
 Output: EN_sreverError_t Error or No Error
 Description: 1. This function will take terminal data and validate these data.
              2. It checks if the transaction's amount is available or not.
              3. If the transaction amount is greater than the balance in the database will return LOW_BALANCE, 
                 else will return SERVER_OK
*/
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
    /* Define local variable to set the error state, No Error */
    EN_serverError_t Loc_ErrorState = SERVER_OK;
    
    /* Check: Transaction Amount > Account Balance */
    if (termData->transAmount > accountRefrence->balance)
    {
        /* Update error state, Low Balance! */
        Loc_ErrorState =  LOW_BALANCE; 
    }

    return Loc_ErrorState;
}

/*
 Name: saveTransaction
 Input: Pointer to Transaction structure
 Output: EN_sreverError_t Error or No Error
 Description: 1. This function will store all transaction data in the transactions database.
              2. It gives a sequence number to a transaction, this number is incremented once a transaction 
                 is processed into the server, you must check the last sequence number in the server to give 
                 the new transaction a new sequence number.
              3. It saves any type of transactions, APPROVED or DECLINED, with the specific reason for declining/transaction state.
              4. If the transaction can't be saved, for any reason (ex: dropped connection) will return SAVING_FAILED, 
                 else will return SERVER_OK, you can simulate this by commenting on the lines where your 
                 code writes the transaction data in the database.
              5. It checks if the transaction is saved or not using the getTransaction function.
*/
EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    /* Define local variable to set the error state, No Error */
    EN_serverError_t Loc_ErrorState = SERVER_OK;
    /* Define static local uint32_t variable to store the sequence number of the current transaction */
    static uint32_t transSeqNumber = 1000;

    /* Save the current sequence number in the current transaction structure */
    transData->transactionSequenceNumber = transSeqNumber;
    /* Save Transaction Data in transactionsDB */
    transactionsDB[Glb_TransactionsDBIndex] = *transData;

    /* Check 1: Transaction is not found */
    if (getTransaction(transSeqNumber, transData) == TRANSACTION_NOT_FOUND)
    {
        /* Update error state, Saving Failed! */
        Loc_ErrorState = SAVING_FAILED;
    }
    /* Check 2: Transaction is found */
    else
    {
        /* Increment transaction sequence number */
        transSeqNumber++;
        /* Increment transactionsDB Index */
        Glb_TransactionsDBIndex++;
    }

    return Loc_ErrorState;
}

/*
 Name: getTransaction
 Input: uint32_t Transaction Number, Pointer to Transaction structure
 Output: EN_sreverError_t Error or No Error
 Description: 1. This function takes the sequence number of a transaction and returns the transaction data 
                 if found in the transactions DB.
              2. If the sequence number is not found, then the transaction is not found, 
                 the function will return TRANSACTION_NOT_FOUND, else return transaction data as well as SERVER_OK
*/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
    /* Define local variable to set the error state, No Error */
    EN_serverError_t Loc_ErrorState = SERVER_OK ;
    /* Define local variable to set the flag state, Flag Down */
    EN_flagState_t Loc_Flag = FLAG_DOWN;

    /* Loop: Until Transaction is found or until the end of transactionsDB */
    for(uint8_t Loc_Index = 0; Loc_Index < 255; Loc_Index++)
    {
        /* Check 1: Transaction is found */
        if (transactionsDB[Loc_Index].transactionSequenceNumber == transactionSequenceNumber)
        {
            /* Copy transaction details from transactionsDB to passed pointer */
            *transData = transactionsDB[Loc_Index];

            /* Update flag state, Transaction Found! */
            Loc_Flag = FLAG_UP;
            break;
        }
    }

    /* Check 2: Transaction not found */
    if (Loc_Flag == FLAG_DOWN)
    {
        /* Update error state, Transaction Not Found! */
        Loc_ErrorState = TRANSACTION_NOT_FOUND;
    }

    return Loc_ErrorState;
}