/************************************************************************************
Title:						Library for 8-bit expander PCF8574 using TWI
*************************************************************************************/

#ifndef __PCF8574_H__
#define __PCF8574_H__

#include <avr/io.h>
#include <stdlib.h>
#include "TWI.h"

class PCF8574 : public ITWI
{
	public:
	
	/***********************************************************************
	Function:		PCF8574()
	Purpose:		Initializing PCF8574
	Input:			- TWI pointer
					- A2, A1, A0 pins state
					- Clock frequency value in Hz
	Returns:		No
	************************************************************************/
	PCF8574(TWI *twi, uint8_t A2, uint8_t A1, uint8_t A0, uint32_t clockFrequency);

	/***********************************************************************
	Function:		StartRead()
	Purpose:		Initializing port reading, the result will be saved in 'data'
	Input:			- Data to receiving
	Returns:		No
	************************************************************************/
	void StartRead(uint8_t &data);
	
	/***********************************************************************
	Function:		SetDataToWrite()
	Purpose:		Copying data in self buffer before transmitting
	Input:			- Data to transmitting
					- Data size in bytes
	Returns:		No
	************************************************************************/
	template <typename T>
	void SetDataToWrite(T &data, uint8_t dataSize);
	

	/***********************************************************************
	Function:		GetTransactionStatus()
	Purpose:		Return current transaction status
	Input:			No
	Returns:		- Transaction status
	************************************************************************/
	virtual TWI_TransactionStatus GetTransactionStatus() override;
		
	~PCF8574();
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	protected:
	
	virtual uint32_t ClockFrequency() override;

	virtual void NextOperation(TWI_Status status) override;
	
	private:
	
	void ClearDataCounter();
	
	void HandleWriteOperation();
	
	void HandleReadOperation();
	
	TWI *twi;
	
	uint8_t address;
	
	uint32_t clockFrequency;
		
	uint8_t *data;
	uint8_t dataSize;
	uint8_t dataCounter;
	
	TWI_Status status;
	
	TWI_Operation operation;
	
	TWI_TransactionStatus transactionStatus;
}; 

template <typename T>
void PCF8574::SetDataToWrite(T &data, uint8_t dataSize)
{
	if(this->transactionStatus == TWI_InProcess) return;

	this->transactionStatus = TWI_InProcess;
	this->operation = TWI_Write;	

	this->dataSize = dataSize;

	ClearDataCounter();
	
	do
	{
		this->data = (uint8_t *)malloc(this->dataSize);
	} while (this->data == nullptr);

	uint8_t *dataPtr = (uint8_t*)&data;

	for(uint8_t i = 0; i < this->dataSize; i++, dataPtr++)
	{
		this->data[i] = *dataPtr;
	}
}

#endif
