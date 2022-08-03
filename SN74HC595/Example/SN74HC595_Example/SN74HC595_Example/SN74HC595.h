/************************************************************************************
Title:						Library for 8-bit shift register SN74HC595
		 
Notes:						- If necessary, additional chip control functions can be 
							  added, such as resetting and activating outputs. If this 
							  is not required, the reset pin of the chip should be 
							  connected to the plus of the power supply, and the 
							  activation pin of the output should be connected to the 
							  minus. These functions work independently of each other.
							- A pointer to SPI_Master object is used for set output
							- If the chips are connected in series, then the last chip 
							  will have the value of the highest byte, while its output 
							  Q7 will have the value of the highest bit. the first chip 
							  of the sequence will have the value of the lowest byte, 
							  and its output Q0 is the value of the lowest bit.
*************************************************************************************/

#ifndef SN74HC595_H
#define SN74HC595_H

#include <avr/io.h>

#include "Port.h"
#include "SPI_Master.h"
#include "Buffer.h"

class SN74HC595 : public ISPI
{
	public:

	/***********************************************************************
	Function:		SN74HC595()
	Purpose:		Initializing shift register
	Input:			- Latch port
	Returns:		No
	************************************************************************/
	SN74HC595(Port *LATCH, uint8_t bufferSize);
	
	/***********************************************************************
	Function:		AddRST()
	Purpose:		Adding a reset port
	Input:			- Reset port
	Returns:		No
	************************************************************************/
	void AddRST(Port *RST);

	/***********************************************************************
	Function:		AddOE()
	Purpose:		Adding a output enable port
	Input:			- Output enable port
	Returns:		No
	************************************************************************/	
	void AddOE(Port *OE);

	/***********************************************************************
	Function:		SetOutput()
	Purpose:		Data sending
	Input:			- Output enable port
	Returns:		No
	************************************************************************/	
	template <typename T>
	void SetOutput(T &data, uint8_t dataSize);
	
	void EnableOutput();
	
	void DisableOutput();
	
	void Reset();	

	~SN74HC595();
	
	void* operator new(size_t size);
		
	void operator delete(void* ptr);

	protected:

	virtual SPI_Polarity Polarity() override;

	virtual SPI_Phase Phase() override;

	virtual SPI_DataOrder DataOrder() override;

	virtual uint8_t DataSize() override;

	virtual uint8_t * Data() override;
	
	virtual void Start() override;
	virtual void Finish() override;

	private:
	
	Port *LATCH;
	Port *OE;
	Port *RST;
	
	Buffer *buffer;	
	
	void Latch();
};

template <typename T>
void SN74HC595::SetOutput(T &data, uint8_t dataSize)
{
	this->buffer->Write(data, dataSize);
}

#endif 