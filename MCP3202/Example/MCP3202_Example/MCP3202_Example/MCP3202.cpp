#include "MCP3202.h"

#define StartBit 1
#define SignBit 1
#define MSBF_Bit 1
#define DontCareBit 0

MCP3202 :: MCP3202(Port* CS, uint8_t channel)
{
	this->channel = channel;
	
	this->CS = CS;
	this->CS->SetAsOutput();
	this->CS->Set();	
}

uint16_t MCP3202 :: GetResult(SPI_Master* spiMaster)
{
	spiMaster->SetMode(RISING, START);								//Set SPI mode	
	
	this->CS->Clear();												//Select chip	
	
	spiMaster->SendByte(StartBit);									//Send start bit
	
	uint8_t configByte = (SignBit<<7)|(channel<<6)|(MSBF_Bit<<5);	
	spiMaster->SendByte(configByte);								//Send config byte
		
	uint16_t result = spiMaster->ReceiveByte();						//First part of result		
	result <<= 12;										
	result >>= 4;	
	
	spiMaster->SendByte(DontCareBit);								//Send don't care bit
		
	result |= spiMaster->ReceiveByte();								//Second part of result
				
	this->CS->Set();												//Deactivate chip
	
	return result;	
}

MCP3202::~MCP3202(){}

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