#include "LTC1864.h"

LTC1864::LTC1864(Port* conv)
{
	this->conv = conv;

	this->conv->SetAsOutput();
	this->conv->Set();
}

uint16_t LTC1864::GetResult(SPI_Master* spiMaster)
{
	spiMaster->SetMode(FALLING, END);
	
	_delay_us(5);
	
	this->conv->Clear();
	
	_delay_us(1);	
		
	spiMaster->SendByte(0);
	
	uint16_t result = (spiMaster->ReceiveByte() << 8);
			
	spiMaster->SendByte(0);
	
	result |= spiMaster->ReceiveByte();
	
	this->conv->Set();
	
	return result;
}

LTC1864::~LTC1864(){}

void* LTC1864::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void LTC1864::operator delete(void* ptr)
{
	free(ptr);
}
