#include "AD5541.h"

AD5541::AD5541(Port* CS, uint8_t bufferSize)
{
	this->buffer = new Buffer(bufferSize);
	
	this->CS = CS;
	
	this->CS->SetAsOutput();
	this->CS->Set();	
}

void AD5541::SetOutput(uint16_t data)
{
	uint8_t _data = data >> 8;
		
	this->buffer->Write(_data, sizeof(uint8_t));
		
	_data = data;
		
	this->buffer->Add(_data, sizeof(uint8_t));	
}

AD5541::~AD5541()
{
	delete this->buffer;
}

void* AD5541::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void AD5541::operator delete(void* ptr)
{
	free(ptr);
}

SPI_Polarity AD5541::Polarity()
{
	return SPI_FALLING;
}

SPI_Phase AD5541::Phase()
{
	return SPI_END;
}

SPI_DataOrder AD5541::DataOrder()
{
	return SPI_MBF;
}

uint8_t AD5541::DataSize()
{
	return this->buffer->BlockSize();
}

uint8_t * AD5541::Data()
{
	return this->buffer->Read();
}

void AD5541::Start()
{
	this->CS->Clear();
}

void AD5541::Finish()
{
	this->CS->Set();
	
	this->buffer->ClearBlock();
}

