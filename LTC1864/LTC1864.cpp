#include "LTC1864.h"

LTC1864::LTC1864(Port* CONV)
{
	this->CONV = CONV;

	this->CONV->SetAsOutput();
	this->CONV->Set();
}

uint16_t LTC1864::GetResult(SPI_Master* spiMaster)
{
	spiMaster->SetMode(FALLING, END);
	
	_delay_us(5);
	
	this->CONV->Clear();
	
	_delay_us(1);	
		
	spiMaster->SendByte(0);
	
	uint16_t result = (spiMaster->ReceiveByte() << 8);
			
	spiMaster->SendByte(0);
	
	result |= spiMaster->ReceiveByte();
	
	this->CONV->Set();
	
	return result;
}
