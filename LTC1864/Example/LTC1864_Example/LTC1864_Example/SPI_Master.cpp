#include "SPI_Master.h"

SPI_Master :: SPI_Master(Port* SCK, Port* MOSI, Port* MISO, Port* SS)
{
	this->SCK = SCK;
	this->MOSI = MOSI;
	this->MISO = MISO;
	this->SS = SS;
	
	this->SCK->SetAsOutput();
	this->MOSI->SetAsOutput();
	this->MISO->Set();
	this->SS->Set();
	this->SS->SetAsOutput();	
		
	SPCR|=(1<<MSTR)|(1<<SPE)|(1<<SPR0)|(1<<SPR1);
}

void SPI_Master::SetMode(SPI_Polarity polarity, SPI_Phase phase)
{
	SPI_Polarity _polarity = (SPI_Polarity)(SPCR & (1<<CPOL));
	if(_polarity != polarity)
	{
		if(polarity == RISING) SPCR &=	~(1<<CPOL);
		else if(polarity == FALLING) SPCR |= (1<<CPOL);
	}
	
	SPI_Phase _phase = (SPI_Phase)(SPCR & (1<<CPHA));
	if(_phase != phase)
	{
		if(phase == START) SPCR &=	~(1<<CPHA);
		else if(phase == END) SPCR |= (1<<CPHA);
	}
}

void SPI_Master :: SendByte(uint8_t data)
{		
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));		
}

uint8_t SPI_Master :: ReceiveByte()
{	
	return SPDR;
}

SPI_Master::~SPI_Master(){}

void* SPI_Master::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void SPI_Master::operator delete(void* ptr)
{
	free(ptr);
}