/************************************************************************************
Title:						Library for EEPROM chip 24X256, 24XX256

Support:					All EEPROM chip 24X256, 24XX256

Notes:						- TWI is used for the data exchange
*************************************************************************************/

#ifndef __AT24C256_H__
#define __AT24C256_H__

#include <avr/io.h>
#include <stdlib.h>
#include "TWI.h"

/************************************************************************************
Description:		Memory size in bytes
*************************************************************************************/
const uint16_t fieldSize = 32768;

class AT24C256 : public ITWI
{	
	public:
	
	/***********************************************************************
	Function:		AT24C256()
	Purpose:		Initializing AT24C256
	Input:			- TWI pointer
					- A2, A1, A0 pins state
					- Clock frequency value in Hz
	Returns:		No
	************************************************************************/
	AT24C256(TWI *twi, uint8_t A2, uint8_t A1, uint8_t A0, uint32_t clockFrequency);
	
	/***********************************************************************
	Function:		SetDataToRead()
	Purpose:		Preparing before data reading operation. Data returns
					implement by pointer
	Input:			- Physical data address in EEPROM chip
					- Data to receiving
					- Data size in bytes
	Returns:		No
	************************************************************************/
	template <typename T>
	void SetDataToRead(uint16_t dataAddress, T &data, uint8_t dataSize);
	
	/***********************************************************************
	Function:		SetDataToWrite()
	Purpose:		Copying data in self buffer before transmitting
	Input:			- Physical data address in EEPROM chip
					- Data to transmitting
					- Data size in bytes
	Returns:		No
	************************************************************************/
	template <typename T>
	void SetDataToWrite(uint16_t dataAddress, T &data, uint8_t dataSize);

	/***********************************************************************
	Function:		GetTransactionStatus()
	Purpose:		Return current transaction status
	Input:			No
	Returns:		- Transaction status
	************************************************************************/
	virtual TWI_TransactionStatus GetTransactionStatus() override;
	
	~AT24C256();	
			
	void* operator new(size_t size);
			
	void operator delete(void* ptr);	
	
	virtual uint32_t ClockFrequency() override;
	
	virtual void NextOperation(TWI_Status status) override;	

	private:
	
	void ClearCounters();
	
	void NormaliseDataRange();
	
	void HandleWriteOperation();
	
	void HandleReadOperation();	
	
	TWI *twi;
	
	uint8_t address;
	
	uint32_t clockFrequency;	
		
	uint16_t dataAddress;
	uint8_t *dataAddressPtr;
	uint8_t dataAddressCounter;			
	
	uint8_t *data;
	uint8_t dataSize;	
	uint8_t dataCounter;
	
	TWI_Status status;
	
	TWI_Operation operation;
	
	TWI_TransactionStatus transactionStatus;
}; 

template <typename T>
void AT24C256::SetDataToRead(uint16_t dataAddress, T &data, uint8_t dataSize)
{
	if(dataAddress >= fieldSize) return;
	
	if(this->transactionStatus == TWI_InProcess) return;
	
	this->transactionStatus = TWI_InProcess;
	this->operation = TWI_Read;
	
	this->dataAddress = dataAddress;
	this->dataAddressPtr = (uint8_t*)&this->dataAddress;
		
	this->dataSize = dataSize;
	
	ClearCounters();
	
	NormaliseDataRange();	
	
	this->data = (uint8_t*)&data;
}

template <typename T>
void AT24C256::SetDataToWrite(uint16_t dataAddress, T &data, uint8_t dataSize)
{	
	if(dataAddress >= fieldSize) return;
	
	if(this->transactionStatus == TWI_InProcess) return;	
	
	this->transactionStatus = TWI_InProcess;
	this->operation = TWI_Write;
		
	this->dataAddress = dataAddress;
	this->dataAddressPtr = (uint8_t*)&this->dataAddress;	
	
	this->dataSize = dataSize;
	
	ClearCounters();
	
	NormaliseDataRange();
			
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

/***********************************************************************
								HOW IT USE
						
- First need create TWI object:

	TWI *twi = new TWI(F_CPU);

- Next, need create AT24C256 object(s):

	AT24C256 *eep = new AT24C256(twi, [physical twi address], clockFrequency);
	...

- Add read or write operation:

	eep->SetDataToRead(...);
	eep->SetDataToWrite(...);
	
	!!! Before adding a read or write operation, you need to make sure that
		the transaction status is TWI_Ready. After the transaction is completed,
		the transaction status can be TWI_Ok or TWI_Error. After reading the 
		transaction status, it will be changed to TWI_Ready, except when the 
		transaction status is TWI_In Process. As long as the transaction status 
		is TWI_In Process, adding data exchange operations is not possible, 
		attempts will be ignored.

************************************************************************/

#endif 
