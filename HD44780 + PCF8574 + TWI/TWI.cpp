#include "TWI.h"

TWI::TWI(uint32_t processorFrequency)
{
	this->processorFrequency = processorFrequency;	
}

void TWI::SendByte(uint32_t bitrate, uint8_t address, uint8_t data)
{
	//setup bitrate	
	if(this->bitrate != bitrate)
	{
		TWBR = ((processorFrequency / bitrate) - 16) / 2;
		this->bitrate = bitrate;
	}
	
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

