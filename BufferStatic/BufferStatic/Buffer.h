/************************************************************************************
Title:						Buffer for data storage
		 
Notes:						- The storage data can be of any type and size
							- Reading is done byte by byte or through a pointer
							- If the buffer is full, no recording is performed
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
	Buffer(uint8_t bufferSize, uint8_t blockSize);
	
	/***********************************************************************
	Function:		BufferSize()
	Purpose:		Return the size of the buffer (blocks quantity)
	Input:			No
	Returns:		Buffer size
	************************************************************************/		
	uint8_t BufferSize();	

	/***********************************************************************
	Function:		Write()
	Purpose:		Writing data to buffer
	Input:			- data
					- data size in bytes
	Returns:		No
	************************************************************************/
	template <typename T>
	void Write(T &data, uint8_t dataSize);
	
	void Save();	
	
	/***********************************************************************
	Function:		Read()
	Purpose:		Reading data from buffer
	Input:			No
	Returns:		Pointer of the data or null pointer if the buffer is empty
	************************************************************************/
	uint8_t *Read();
	
	void Next();

	/***********************************************************************
	Function:		BlockSize()
	Purpose:		Return the size of the readable block
	Input:			No
	Returns:		Block size
	************************************************************************/	
	uint8_t BlockSize();
	
	~Buffer();
			
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
	
	private:
	
	uint8_t bufferSize;
	uint8_t blockSize;
	
	uint8_t **buffer;
	uint8_t *dataSize;
	
	uint8_t bufferCounter;
	uint8_t writePtr;
	uint8_t readPtr;
}; 


template <typename T>
void Buffer::Write(T &data, uint8_t dataSize)
{	
	if(this->bufferCounter == this->bufferSize) return;	
	
	uint8_t newDataSize = this->dataSize[this->writePtr] + dataSize;
	
	if(newDataSize >= this->blockSize) return;
	
	memcpy(this->buffer[this->writePtr] + this->dataSize[this->writePtr], &data, dataSize);
	
	this->dataSize[this->writePtr] = newDataSize;	
}

/***********************************************************************
								HOW IT USE

Buffer *buf = new Buffer(someSize);

************************************************************************						
							-=Save data to a buffer=-

SomeType data...

buf->Write(data, sizeof(data));

Memory block: |d|a|t|a|

Also you can add data in current block of the buffer:

SomeType data_2...

buf->Write(data_2, sizeof(data));

Memory block: |d|a|t|a|d|a|t|a|_|2|

Complete changes:

buf->Save(); <-IT'S NESSESARY

************************************************************************								
							-=Read data from buffer=-							
	
uint8_t *data = buf->Read();

...do something...

Or

if(someCounter < buf->BlockSize())
{
	uint8_t data = buf->Read()[someCounter];
	
	...do something...
}

buf->ClearBlock();

!!!When reading a block of data is completed, you need
to call the ClearBlock() function to delete it

************************************************************************/
#endif //__BUFFER_H__
