/************************************************************************************
Title:						SPI Master library 

Supported microcontrollers: All ATmega
		 
Notes:						- The SS pin is set as output
							- By default transferring mode is set in mode 0
							  (polarity - rising, phase - start)
							- SCK frequency = F_CPU / 128
*************************************************************************************/

#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <avr/io.h>
#include "Port.h"

enum SPI_Polarity
{
	RISING,
	FALLING	
};

enum SPI_Phase
{
	START,
	END
};

class SPI_Master
{
	public:
	
	/***********************************************************************
	Function:		SPI_Master()
	Purpose:		Initializing SPI in master mode
	Input:			- SPI pins
	Returns:		No
	************************************************************************/
	SPI_Master(Port* SCK, Port* MOSI, Port* MISO, Port* SS);	
		
	/***********************************************************************
	Function:		SetMode()
	Purpose:		Setting SPI transferring mode
	Input:			- SPI polarity value
					- SPI phase value
	Returns:		No
	************************************************************************/
	void SetMode(SPI_Polarity polarity, SPI_Phase phase);	
		
	/***********************************************************************
	Function:		SendByte()
	Purpose:		Sending byte by SPI
	Input:			- Data to send
	Returns:		No
	************************************************************************/
	void SendByte(uint8_t data);
	
	/***********************************************************************
	Function:		ReceiveByte()
	Purpose:		Receiving byte from SPI
	Input:			No
	Returns:		Received data from SPI
	************************************************************************/
	uint8_t ReceiveByte();
		
	private:
	
	Port* SCK;
	Port* MOSI;
	Port* MISO;
	Port* SS;
};

#endif /* SPI_MASTER_H_ */