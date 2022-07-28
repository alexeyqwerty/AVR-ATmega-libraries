#include "PCF8574.h"

PCF8574::PCF8574(TWI *twi, uint8_t A2, uint8_t A1, uint8_t A0, uint32_t clockFrequency)
{
	this->twi = twi;
	 
	this->address = 0b01000000 | (A2 << 3) | (A1 << 2) | (A0 << 1);
	
	this->clockFrequency = clockFrequency;
	
	this->data = nullptr;
	
	this->status = TWI_NoActions;
	
	this->operation = TWI_NoOperation;
	
	this->transactionStatus = TWI_Ready;
}

void PCF8574::StartRead(uint8_t &data)
{
	if(this->transactionStatus == TWI_InProcess) return;

	this->transactionStatus = TWI_InProcess;
	this->operation = TWI_Read;	

	this->data = (uint8_t*)&data;	
}

uint32_t PCF8574::ClockFrequency()
{
	return this->clockFrequency;
}

TWI_TransactionStatus PCF8574::GetTransactionStatus()
{
	TWI_TransactionStatus temp = this->transactionStatus;
	
	if(this->transactionStatus != TWI_InProcess)
		this->transactionStatus = TWI_Ready;
	
	return temp;
}

void PCF8574::NextOperation(TWI_Status status)
{
	this->status = status;
	
	switch(this->status)
	{
		case TWI_NoActions: return;
		
		case TWI_ArbitrationLost:
		{
			this->twi->Stop();
			
			ClearDataCounter();
			
			this->twi->Start();
		}return;
		
		case TWI_BusError:
		case TWI_MasterAddressWriteNACK:
		case TWI_MasterDataWriteNACK:
		case TWI_MasterAddressReadNACK:
		{
			this->transactionStatus = TWI_Error;
			
			this->twi->DisableInterrupt();
		}return;
		
		default: break;
	}
	
	if(this->operation == TWI_Write) HandleWriteOperation();
	else if(this->operation == TWI_Read) HandleReadOperation();
}

PCF8574::~PCF8574()
{
	free((uint8_t*)data);
}

void* PCF8574::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void PCF8574::operator delete(void* ptr)
{
	if(ptr != nullptr) free(ptr);
}

void PCF8574::ClearDataCounter()
{
	this->dataCounter = 0;
}

void PCF8574::HandleWriteOperation()
{
	switch(this->status)
	{
		case TWI_Start:
		{
			this->twi->WriteToTWDR(this->address);
		}break;
		
		case TWI_MasterAddressWriteACK:
		case TWI_MasterDataWriteACK:
		{
			if(this->dataCounter < this->dataSize)
			{
				this->twi->WriteToTWDR(this->data[this->dataCounter]);
				this->dataCounter++;
			}
			
			else
			{
				this->twi->Stop();
				
				this->operation = TWI_NoOperation;
				
				free((uint8_t*)this->data);
				
				this->transactionStatus = TWI_Ok;
			}			
		}break;
		
		default: break;
	}
}

void PCF8574::HandleReadOperation()
{
	switch(this->status)
	{
		case TWI_Start:
		{
			this->twi->WriteToTWDR(this->address | 1);
		}break;	
		
		case TWI_MasterAddressReadACK:		
		this->twi->ACK();break;
		case TWI_MasterDataReadACK:
		{		
			
				*this->data = this->twi->ReadFromTWDR();			
			
				this->twi->NACK();
			
		
		}break;
	
		case TWI_MasterDataReadNACK:
		{		
			this->twi->Stop();
		
			this->operation = TWI_NoOperation;
		
			this->transactionStatus = TWI_Ok;
		}break;
	
		default: break;
	}
}
