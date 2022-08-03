#include "MCP3202.h"

#define StartBit 1
#define SignBit 1
#define MSBF_Bit 1
#define DontCareBit 0

MCP3202 :: MCP3202(Port* CS, uint8_t channel)
{
	this->channel = channel;
	
	this->buffer = new Buffer(1);
	
	this->CS = CS;
	this->CS->SetAsOutput();
	this->CS->Set();	
	
	this->result = 0;
	
	PrepareDataChange();
}

uint16_t MCP3202 :: GetResult()
{		
	return this->result;	
}

MCP3202::~MCP3202()
{
	delete this->buffer;
}

void* MCP3202::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void MCP3202::operator delete(void* ptr)
{
	free(ptr);
}

SPI_Polarity MCP3202::Polarity()
{
	return SPI_RISING;
}

SPI_Phase MCP3202::Phase()
{
	return SPI_START; 
}

SPI_DataOrder MCP3202::DataOrder()
{
	return SPI_MBF;
}

uint8_t MCP3202::DataSize()
{
	return this->buffer->BlockSize();
}

uint8_t * MCP3202::Data()
{
	return this->buffer->Read();
}

void MCP3202::Start()
{
	this->CS->Clear();	
}

void MCP3202::Finish()
{
	this->CS->Set();
	
	uint8_t *ptr = this->buffer->Read();
	
	ptr++;	
		
	this->result = *ptr;
	this->result <<= 12;
	this->result >>= 4;
	
	ptr++;
	
	this->result |= *ptr;
	
	this->buffer->ClearBlock();
	
	PrepareDataChange();
}

void MCP3202::PrepareDataChange()
{
	uint8_t data = StartBit;
	buffer->Write(data, sizeof(uint8_t));

	data = (SignBit<<7)|(channel<<6)|(MSBF_Bit<<5);
	buffer->Add(data, sizeof(uint8_t));
	
	data = DontCareBit;
	buffer->Add(data, sizeof(uint8_t));
}
