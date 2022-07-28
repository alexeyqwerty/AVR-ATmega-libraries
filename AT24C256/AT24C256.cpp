#include "AT24C256.h"

AT24C256::AT24C256(TWI *twi, uint8_t A2, uint8_t A1, uint8_t A0, uint32_t clockFrequency)
{
	this->twi = twi;
	
	this->address = 0b10100000 | (A2 << 3) | (A1 << 2) | (A0 << 1);
	
	this->clockFrequency = clockFrequency;
	
	this->dataAddressPtr = (uint8_t*)&this->dataAddress;
	
	this->data = nullptr;
	
	this->status = TWI_NoActions;
	
	this->operation = TWI_NoOperation;
	
	this->transactionStatus = TWI_Ready;
}

uint32_t AT24C256::ClockFrequency()
{
	return this->clockFrequency;
}

void AT24C256::NextOperation(TWI_Status status)
{	
	this->status = status;
	
	switch(this->status)
	{
		case TWI_NoActions: return;
		
		case TWI_ArbitrationLost:		
		{
			this->twi->Stop();
			
			ClearCounters();
			
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

TWI_TransactionStatus AT24C256::GetTransactionStatus()
{
	TWI_TransactionStatus temp = this->transactionStatus;
	
	if(this->transactionStatus != TWI_InProcess)
		this->transactionStatus = TWI_Ready;
	
	return temp;
}

AT24C256::~AT24C256()
{
	free((uint8_t*)data);
}

void* AT24C256::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void AT24C256::operator delete(void* ptr)
{
	if(ptr != nullptr) free(ptr);
}

void AT24C256::ClearCounters()
{
	this->dataAddressCounter = sizeof(this->dataAddress);
	this->dataCounter = 0;
}

void AT24C256::NormaliseDataRange()
{
	if((this->dataAddress + this->dataSize) > fieldSize)
	{
		this->dataSize = fieldSize - this->dataAddress;
	}
}

void AT24C256::HandleWriteOperation()
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
			if(this->dataAddressCounter > 0)
			{
				this->twi->WriteToTWDR(this->dataAddressPtr[this->dataAddressCounter - 1]);
				
				this->dataAddressCounter--;
			}
			
			else
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
			}
		}break;
		
		default: break;
	}
}

void AT24C256::HandleReadOperation()
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
			if(this->dataAddressCounter > 0)
			{
				this->twi->WriteToTWDR(this->dataAddressPtr[this->dataAddressCounter - 1]);
				
				this->dataAddressCounter--;
			}
			
			else
			{
				this->twi->Start();
			}
		}break;
		
		case TWI_Restart:
		{
			this->twi->WriteToTWDR(this->address | 1);
		}break;
				
		case TWI_MasterDataReadACK:
		{
			this->data[this->dataCounter] = this->twi->ReadFromTWDR();
			this->dataCounter++;
		}
		case TWI_MasterAddressReadACK:
		{
			if(this->dataCounter < this->dataSize - 1)
			{
				this->twi->ACK();
			} 
			else
			{
				this->twi->NACK();
			}
								
		}break;
		
		case TWI_MasterDataReadNACK:
		{
			this->data[this->dataCounter] = this->twi->ReadFromTWDR();
			
			this->data = nullptr;
			
			this->twi->Stop();		
			
			this->operation = TWI_NoOperation;	
			
			this->transactionStatus = TWI_Ok;
		}break;
		
		default: break;
	}
}