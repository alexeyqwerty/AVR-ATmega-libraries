#include "AD5541.h"

AD5541::AD5541(Port* CS)
{
	this->CS = CS;
	
	this->CS->SetAsOutput();
	this->CS->Set();	
}

void AD5541::SetOutput(uint16_t data, SPI_Master* spiMaster)
{
	spiMaster->SetMode(FALLING, END);
	
	this->CS->Clear();
	
	spiMaster->SendByte(data >> 8);
	spiMaster->ReceiveByte();
	spiMaster->SendByte(data);
	spiMaster->ReceiveByte();
	
	this->CS->Set();
}

