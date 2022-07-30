#include "Buffer.h"

Buffer::Buffer(uint8_t bufferSize)
{
	this->bufferSize = bufferSize;
	this->bufferCounter = 0;
	this->dataCounter = 0;
	this->writePtr = 0;
	this->readPtr = 0;
	
	do 
	{
		this->buffer = (uint8_t **)malloc(this->bufferSize * sizeof(uint8_t*));
	} while(this->buffer == nullptr);	
	
	memset(this->buffer, 0, this->bufferSize * sizeof(uint8_t*));
	
	do 
	{
		this->dataSize = (uint8_t *)malloc(this->bufferSize);
	} while(this->dataSize == nullptr);
}

uint8_t Buffer::Read()
{
	uint8_t data = *(this->buffer[this->readPtr] + this->dataCounter);
	
	this->dataCounter++;	
	
	if(this->dataCounter == this->dataSize[this->readPtr])	
	{
		this->dataCounter = 0;
		
		free((uint8_t*)this->buffer[this->readPtr]);
		
		++this->readPtr %= this->bufferSize;
		
		this->bufferCounter--;
	}
	
	return data;
}

bool Buffer::Empty()
{
	return this->bufferCounter == 0;
}

Buffer::~Buffer()
{
	for(uint8_t i = 0; i < this->bufferSize; i++)
	{
		free((uint8_t*)this->buffer[i]);
	}
	
	free((uint8_t**)this->buffer);
	free((uint8_t*)this->dataSize);
}

void* Buffer::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void Buffer::operator delete(void* ptr)
{
	free(ptr);
}
