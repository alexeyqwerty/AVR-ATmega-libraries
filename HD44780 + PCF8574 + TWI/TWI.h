/************************************************************************************
Title:						Library for TWI (I2C) without using interrupts
*************************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

class TWI
{
	public:
	/***********************************************************************
	Function:		TWI()
	Purpose:		Initializing TWI
	Input:			- Processor tact frequency value
	Returns:		No
	************************************************************************/
	TWI(uint32_t processorFrequency);

	/***********************************************************************
	Function:		SendByte()
	Purpose:		Sending data byte
	Input:			- Bitrate for the receiving device
					- Address receiving device
					- Data
	Returns:		No
	************************************************************************/
	void SendByte(uint32_t bitrate, uint8_t address, uint8_t data);	
	
	private:
	
	uint32_t processorFrequency;
	
	uint32_t bitrate;
};

#endif