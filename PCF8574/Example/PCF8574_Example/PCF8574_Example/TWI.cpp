#include "TWI.h"

TWI::TWI(uint32_t processorFrequency)
{
	this->processorFrequency = processorFrequency;
	
	this->devicesQuantiy = 0;
	this->selectedDevice = 0;
}

void TWI::AddDevice(ITWI *device)
{
	if(devicesQuantiy < TWI_DEVICES)
	{
		devices[devicesQuantiy] = device;
		devicesQuantiy++;
	}
}

void TWI::SetClockFrequency(uint32_t clockFrequency)
{
	if(this->clockFrequency != clockFrequency)
	{
		TWBR = ((processorFrequency / clockFrequency) - 16) / 2;
		this->clockFrequency = clockFrequency;
	}
}

void TWI::CheckDevices()
{
	if(TWCR & (1<<TWIE)) return;
	
	if(devices[selectedDevice]->GetTransactionStatus() == TWI_InProcess)
	{
		SetClockFrequency(devices[selectedDevice]->ClockFrequency());
		
		Start();		
	}
	
	else
	{
		++selectedDevice %= devicesQuantiy;
	}
}

void TWI::HandleDataChange()
{	
	devices[selectedDevice]->NextOperation((TWI_Status)(TWSR & TWI_NoActions));
}

void TWI::Start()
{
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA);
}

void TWI::WriteToTWDR(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
}

void TWI::ACK()
{
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
}

void TWI::NACK()
{
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT); 
}

uint8_t TWI::ReadFromTWDR()
{
	uint8_t data = TWDR;
	return data;
}

void TWI::Stop()
{
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO)|(0<<TWIE);
	
	devices[selectedDevice]->NextOperation(TWI_NoActions);
	
	++selectedDevice %= devicesQuantiy;
}

void TWI::DisableInterrupt()
{
	TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);
	
	++selectedDevice %= devicesQuantiy;
}

TWI::~TWI(){}

void* TWI::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void TWI::operator delete(void* ptr)
{
	free(ptr);
}