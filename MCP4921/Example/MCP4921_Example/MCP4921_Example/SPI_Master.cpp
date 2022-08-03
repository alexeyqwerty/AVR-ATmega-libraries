#include "SPI_Master.h"

SPI_Master :: SPI_Master(Port* SCK, Port* MOSI, Port* MISO, Port* SS)
{	
	this->devices = new Devices<ISPI>();
	
	this->spiBusy = false;
		
	SCK->SetAsOutput();
	SCK->Clear();
	
	MOSI->SetAsOutput();
	MOSI->Clear();
	
	MISO->SetAsInput();
	MISO->Set();	
	
	SS->SetAsOutput();
	SS->Set();	
	
	SPCR|=(1<<MSTR)|(1<<SPE)|(1<<SPIE)|(1<<SPR1);
}

void SPI_Master::AddDevice(ISPI *device)
{
	this->devices->Add(device);
}

void SPI_Master::NextOperation()
{
	this->data[this->dataCounter++] = SPDR;
	
	if(this->dataCounter < this->dataSize)
	{
		SPDR = this->data[this->dataCounter];
	}
	
	else
	{
		this->devices->CurrentDevice()->Finish();
		
		this->spiBusy = false;
		
		this->devices->Next();
	}
}

void SPI_Master::CheckDevices()
{
	if(!this->spiBusy)
	{
		this->dataSize = this->devices->CurrentDevice()->DataSize();
		
		if(this->dataSize > 0)
		{			
			this->data = this->devices->CurrentDevice()->Data();
			this->dataCounter = 0;
			
			SPI_DataOrder dataOrder = this->devices->CurrentDevice()->DataOrder();
			if(dataOrder == SPI_MBF) SPCR &= ~(1<<DORD);
			else if(dataOrder == SPI_LBF) SPCR |= (1<<DORD);
			
			SPI_Phase phase = this->devices->CurrentDevice()->Phase();
			if(phase == SPI_START) SPCR &= ~(1<<CPHA);
			else if(phase == SPI_END) SPCR |= (1<<CPHA);
			
			SPI_Polarity polarity = this->devices->CurrentDevice()->Polarity();
			if(polarity == SPI_RISING) SPCR &= ~(1<<CPOL);
			else if(polarity == SPI_FALLING) SPCR |= (1<<CPOL);
			
			this->devices->CurrentDevice()->Start();
			
			SPDR = this->data[this->dataCounter];
			
			this->spiBusy = true;
		}
		
		else this->devices->Next();
	}
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