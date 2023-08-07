#ifndef SERVER_H_
#define SERVER_H_

/* Library Module */
#include "../Library/standard_types.h"

typedef enum EN_flagState_t
{
	FLAG_DOWN, FLAG_UP
}EN_flagState_t;

typedef enum EN_transState_t 
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR 
}EN_transState_t; 

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData; 
	ST_terminalData_t terminalData; 
	EN_transState_t transState; 
	uint32_t transactionSequenceNumber; 
}ST_transaction_t;

typedef enum EN_serverError_t 
{
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT
}EN_serverError_t ; 

typedef enum EN_accountState_t 
{
	RUNNING, BLOCKED 
}EN_accountState_t;

typedef struct ST_accountsDB_t
{ 
	float32_t balance;
	EN_accountState_t state; 
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

/* Functions' Prototypes */
EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence);
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

#endif /* SERVER_H_ */