#include "Port.h"

Port::Port(volatile uint8_t* PORT, volatile uint8_t* DDR, volatile uint8_t* PIN, uint8_t pinNum)
{
	this->PORT = PORT;
	this->DDR = DDR;
	this->PIN = PIN;
	this->pinNum = pinNum;	
		
	SetAsInput();
	Clear();
}

void Port::SetAsInput()
{
	*this->DDR &= ~(1<<this->pinNum);
}

void Port::SetAsOutput()
{
	*this->DDR |= (1<<this->pinNum);
}

void Port::Set()
{
	*this->PORT |= (1<<this->pinNum);
}

void Port::Clear()
{
	*this->PORT &= ~(1<<this->pinNum);
}

bool Port::IsSet()
{
	return *this->PIN & (1<<this->pinNum);
}

bool Port::IsClear()
{
	return !(*this->PIN & (1<<this->pinNum));
}

Port::~Port(){}

void* Port::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void Port::operator delete(void* ptr)
{
	free(ptr);
}
