#include "TWI.h"

TWI::TWI(uint32_t processorFrequency)
{
	this->processorFrequency = processorFrequency;

	SetBitRate(100000);
}

void TWI::SetBitRate(uint32_t bitRate)
{	
	if(this->bitRate != bitRate)
	{
		TWBR = ((processorFrequency / bitRate) - 16) / 2;
		this->bitRate = bitRate;
	}
}

void TWI::SendByte(uint8_t address, uint8_t data)
{	
	//START
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	while(!(TWCR&(1<<TWINT)));
	
	//address
	TWDR = address;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	
	//data	
	TWDR = data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	
	//STOP
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	
}
