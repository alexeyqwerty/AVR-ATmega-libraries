#include "LTC1864.h"

LTC1864::LTC1864(Port* CS)
{
	this->buffer = new Buffer(1);
	
	this->CS = CS;

	this->CS->SetAsOutput();
	this->CS->Set();
	
	PrepareDataChange();
}

uint16_t LTC1864::GetResult()
{	
	return this->result;
}

LTC1864::~LTC1864()
{
	delete this->buffer;
}

void* LTC1864::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void LTC1864::operator delete(void* ptr)
{
	free(ptr);
}

SPI_Polarity LTC1864::Polarity()
{
	return SPI_FALLING;
}

SPI_Phase LTC1864::Phase()
{
	return SPI_END;
}

SPI_DataOrder LTC1864::DataOrder()
{
	return SPI_MBF;
}

uint8_t LTC1864::DataSize()
{
	return this->buffer->BlockSize();
}

uint8_t * LTC1864::Data()
{
	return this->buffer->Read();
}

void LTC1864::Start()
{
	this->CS->Clear();
}

void LTC1864::Finish()
{
	this->CS->Set();
		
	uint8_t *ptr = this->buffer->Read();
	
	this->result = *ptr;
	this->result <<= 8;
		
	ptr++;
		
	this->result |= *ptr;
		
	this->buffer->ClearBlock();
		
	PrepareDataChange();
}

void LTC1864::PrepareDataChange()
{
	uint16_t data = 0;
	this->buffer->Write(data, sizeof(uint16_t));
}
