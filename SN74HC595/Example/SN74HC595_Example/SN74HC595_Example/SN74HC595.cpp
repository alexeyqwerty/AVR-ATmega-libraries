#include "SN74HC595.h"

 SN74HC595::SN74HC595(Port *LATCH, uint8_t bufferSize)
 { 
	this->LATCH = LATCH;
	this->LATCH->SetAsOutput();
	this->LATCH->Clear();
	
	this->buffer = new Buffer(bufferSize);
 }


void SN74HC595::AddRST(Port *RST)
{
	this->RST = RST;
	
	Reset();
}

void SN74HC595::AddOE(Port *OE)
{
	this->OE = OE;
	this->OE->SetAsOutput();
	this->OE->Clear();
}

void SN74HC595::EnableOutput()
{
	if(this->OE != nullptr)	this->OE->Clear();
}

void SN74HC595::DisableOutput()
{
	if(this->OE != nullptr)	this->OE->Set();
}

void SN74HC595::Reset()
{
	if(this->RST == nullptr) return;
	
	this->RST->Clear();
	
	Latch();
	
	this->RST->Set();
}

SN74HC595::~SN74HC595()
{
	delete this->buffer;
}

void* SN74HC595::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void SN74HC595::operator delete(void* ptr)
{
	free(ptr);
}

SPI_Polarity SN74HC595::Polarity()
{
	return SPI_RISING;
}

SPI_Phase SN74HC595::Phase()
{
	return SPI_START;
}

SPI_DataOrder SN74HC595::DataOrder()
{
	return SPI_MBF;
}

uint8_t SN74HC595::DataSize()
{
	return this->buffer->BlockSize();	
}

uint8_t * SN74HC595::Data()
{
	return this->buffer->Read();
}

void SN74HC595::Start(){}

void SN74HC595::Finish()
{
	Latch();	
	
	this->buffer->ClearBlock();
}

void SN74HC595::Latch()
{
	this->LATCH->Clear();
	this->LATCH->Set();
}
