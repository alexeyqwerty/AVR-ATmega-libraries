#include "MCP4921.h"

#define AB 0
#define BUF 0
#define GA 1
#define SHDN 1

MCP4921::MCP4921(Port *CS, Port *LDAC)
{
	this->CS = CS;
	
	this->CS->SetAsOutput();
	this->CS->Set();
	
	this->LDAC = LDAC;
		
	this->LDAC->SetAsOutput();
	this->LDAC->Set();	
}

void MCP4921 ::SetOutput(uint16_t data, SPI_Master* spiMaster)
{	
	uint8_t _data = data >> 8;
	_data |= (AB<<7)|(BUF<<6)|(GA<<5)|(SHDN<<4);	

	spiMaster->SetMode(RISING, START);
	
	this->CS->Clear();
	
	spiMaster->SendByte(_data);
	spiMaster->ReceiveByte();	
	
	_data = data;
	
	spiMaster->SendByte(_data);		
	spiMaster->ReceiveByte();	
	
	this->CS->Set();
	
	this->LDAC->Clear();
	this->LDAC->Set();
}

MCP4921::~MCP4921() {}

void* MCP4921::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void MCP4921::operator delete(void* ptr)
{
	free(ptr);
}

