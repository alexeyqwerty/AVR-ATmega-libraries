#include "PCF8574.h"

PCF8574::PCF8574(uint8_t A0, uint8_t A1, uint8_t A2, TWI* twi)
{
	this->address = (0b0100<<4)|(A2<<3)|(A1<<2)|(A0<<1);
	
	this->twi = twi;
	
	SetOutput(0);
}

void PCF8574::SetOutput(uint8_t data)
{
	this->twi->SetBitRate(PCF8574_BITRATE);
	
	this->twi->SendByte(address, data);
}