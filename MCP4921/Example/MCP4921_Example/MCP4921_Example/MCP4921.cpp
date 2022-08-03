#include "MCP4921.h"

#define AB 0
#define BUF 0
#define GA 1
#define SHDN 1

MCP4921::MCP4921(Port *CS, Port *LDAC, uint8_t bufferSize)
{
	this->buffer = new Buffer(bufferSize);
	
	this->CS = CS;
	
	this->CS->SetAsOutput();
	this->CS->Set();
	
	this->LDAC = LDAC;
		
	this->LDAC->SetAsOutput();
	this->LDAC->Set();	
}

void MCP4921 ::SetOutput(uint16_t data)
{
	data &= 0xFFF;
	
	uint8_t _data = data >> 8;
	_data |= (AB<<7)|(BUF<<6)|(GA<<5)|(SHDN<<4);
	
	this->buffer->Write(_data, sizeof(uint8_t));
	
	_data = data;
	
	this->buffer->Add(_data, sizeof(uint8_t));
}

MCP4921::~MCP4921()
{
	delete this->buffer;
}

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

SPI_Polarity MCP4921::Polarity()
{
	return SPI_RISING;
}

SPI_Phase MCP4921::Phase()
{
	return SPI_START;
}

SPI_DataOrder MCP4921::DataOrder()
{
	return SPI_MBF;
}

uint8_t MCP4921::DataSize()
{
	return this->buffer->BlockSize();
}

uint8_t * MCP4921::Data()
{
	return this->buffer->Read();
}

void MCP4921::Start()
{
	this->CS->Clear();
}

void MCP4921::Finish()
{
	this->CS->Set();
	
	this->LDAC->Clear();
	this->LDAC->Set();
	
	this->buffer->ClearBlock();
}

