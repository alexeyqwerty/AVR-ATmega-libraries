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

uint8_t *Buffer::Read()
{
	return this->buffer[this->readPtr];
}

uint8_t Buffer::BlockSize()
{
	return this->bufferCounter > 0 ? this->dataSize[this->readPtr] : 0;
}

void Buffer::ClearBlock()
{
	free((uint8_t*)this->buffer[this->readPtr]);

	++this->readPtr %= this->bufferSize;

	this->bufferCounter--;
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
