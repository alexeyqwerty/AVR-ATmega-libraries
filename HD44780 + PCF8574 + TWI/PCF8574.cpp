#include "PCF8574.h"

uint32_t bitrate = 100000;	//Hz

PCF8574::PCF8574(uint8_t A0, uint8_t A1, uint8_t A2, TWI* twi)
{
	this->address = (0b0100<<4)|(A2<<3)|(A1<<2)|(A0<<1);
	
	this->twi = twi;
	
	SetOutput(0);
}

void PCF8574::SetOutput(uint8_t data)
{
	this->twi->SendByte(bitrate, address, data);
}
