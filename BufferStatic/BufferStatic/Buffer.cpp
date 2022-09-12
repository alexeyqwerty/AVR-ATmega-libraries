#include "Buffer.h"

Buffer::Buffer(uint8_t bufferSize, uint8_t blockSize)
{
	this->bufferSize = bufferSize;
	this->blockSize = blockSize;
	
	this->bufferCounter = 0;
	this->writePtr = 0;
	this->readPtr = 0;
	
	do
		this->buffer = (uint8_t**)malloc(this->bufferSize * sizeof(uint8_t*));	
	while(this->buffer == nullptr);
	
	for(uint8_t i = 0; i < this->bufferSize; i++)
	{
		do
			this->buffer[i] = (uint8_t*)malloc(this->blockSize * sizeof(uint8_t));
		while(this->buffer[i] == nullptr);
	}
	
	do
		this->dataSize = (uint8_t*)malloc(this->bufferSize * sizeof(uint8_t));
	while(this->dataSize == nullptr);
	
	for(uint8_t i = 0; i < this->bufferSize; i++)
	{
		this->dataSize[i] = 0;
	}
}

uint8_t Buffer::BufferSize()
{
	return this->bufferSize;	
}

void Buffer::Save()
{
	if(this->bufferCounter == this->bufferSize) return;
	
	++this->writePtr %= this->bufferSize;	
	
	this->bufferCounter++;
}

uint8_t *Buffer::Read()
{
	return this->bufferCounter > 0 ? this->buffer[this->readPtr] : nullptr;
}

void Buffer::Next()
{
	if(this->bufferCounter == 0) return;
	
	this->dataSize[this->readPtr] = 0;
	
	++this->readPtr %= this->bufferSize;
	this->bufferCounter--;
}

uint8_t Buffer::BlockSize()
{
	return this->bufferCounter > 0 ? this->dataSize[this->readPtr] : 0;
}

Buffer::~Buffer()
{
	for(uint8_t i = 0; i < this->bufferSize; i++)
	{
		free(this->buffer[i]);
		this->buffer[i] = nullptr;
	}
	
	free(this->buffer);
	this->buffer = nullptr;
	
	free(this->dataSize);
	this->dataSize = nullptr;
}

void* Buffer::operator new(size_t size)
{
	void* ptr;
	
	do
	ptr = malloc(size);
	while(ptr == nullptr);
	
	return ptr;
}

void Buffer::operator delete(void* ptr)
{
	free(ptr);
	ptr = nullptr;
}
