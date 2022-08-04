/************************************************************************************
Title:						Buffer for data storage
		 
Notes:						- The storage data can be of any type and size
							- Reading is done byte by byte
							- Buffer overflow is not monitored
*************************************************************************************/
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

class Buffer
{
	public:

	/***********************************************************************
	Function:		Buffer()
	Purpose:		Initializing buffer
	Input:			- buffer size in memory blocks
	Returns:		No
	************************************************************************/
	Buffer(uint8_t bufferSize);
	
	/***********************************************************************
	Function:		Write()
	Purpose:		Writing data to buffer
	Input:			- data
					- data size in bytes
	Returns:		No
	************************************************************************/
	template <typename T>
	void Write(T &data, uint8_t dataSize);
	
	/***********************************************************************
	Function:		Add()
	Purpose:		Adding data after using the Write() function
	Input:			- data
					- data size in bytes
	Returns:		No
	************************************************************************/
	template <typename T>
	void Add(T &data, uint8_t dataSize);

	/***********************************************************************
	Function:		Read()
	Purpose:		Reading data from buffer
	Input:			No
	Returns:		Data byte
	************************************************************************/
	uint8_t *Read();

	/***********************************************************************
	Function:		BlockSize()
	Purpose:		Return current block size
	Input:			No
	Returns:		Block size
	************************************************************************/	
	uint8_t BlockSize();

	/***********************************************************************
	Function:		ClearBlock()
	Purpose:		Clear current block
	Input:			No
	Returns:		No
	************************************************************************/		
	void ClearBlock();
	
	~Buffer();
			
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
	
	private:
	
	uint8_t bufferSize;
	uint8_t **buffer;
	uint8_t *dataSize;
	
	uint8_t bufferCounter;
	uint8_t writePtr;
	uint8_t readPtr;
}; 

template <typename T>
void Buffer::Write(T &data, uint8_t dataSize)
{	
	do 
	{
		this->buffer[this->writePtr] = (uint8_t *)malloc(dataSize);
	} while (this->buffer[this->writePtr] == nullptr);
	
	this->dataSize[this->writePtr] = dataSize;	
	
	memcpy(this->buffer[this->writePtr], &data, dataSize);
	
	++this->writePtr %= this->bufferSize;
	this->bufferCounter++;
}

template <typename T>
void Buffer::Add(T &data, uint8_t dataSize)
{
	if(this->writePtr == 0) this->writePtr = this->bufferSize - 1;
	else this->writePtr--;	
		
	if(this->buffer[this->writePtr] != nullptr)
	{
		uint8_t newDataSize = this->dataSize[this->writePtr] + dataSize;
		
		uint8_t *ptr;
		do 
		{
			ptr = (uint8_t*)realloc(this->buffer[this->writePtr], newDataSize);
		}while(ptr == nullptr);
		
		this->buffer[this->writePtr] = ptr;
		
		memcpy(this->buffer[this->writePtr] + this->dataSize[this->writePtr], &data, dataSize);
		
		this->dataSize[this->writePtr] = newDataSize;
	}
	
	++this->writePtr %= this->bufferSize;
}

/***********************************************************************
								HOW IT USE

Buffer *buf = new Buffer(someSize);
						
							-=Save data to a buffer=-

SomeType data...

buf->Write(data, sizeof(data));

							-=Add data to a buffer=-

SomeType data...

buf->Add(data, sizeof(data));
							
!!!Do not use this function before the Write() function. Only after
						
							-=Read data from buffer=-							
	
uint8_t *data = buf->Read();

...do something...

buf->ClearBlock();

!!!When reading a block of data is completed, you need
to call the ClearBlock() function to delete it

************************************************************************/
#endif //__BUFFER_H__
