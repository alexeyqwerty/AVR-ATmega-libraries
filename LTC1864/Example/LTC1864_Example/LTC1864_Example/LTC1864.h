/************************************************************************************
Title:						16 bit ADC LTC1864 library
		 
Notes:						- A pointer to SPI_Master object is used for data exchange
*************************************************************************************/

#ifndef LTC1864_H_
#define LTC1864_H_

#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "SPI_Master.h"
#include "Port.h"

class LTC1864
{
	public:
	
	/***********************************************************************
	Function:		LTC1864()
	Purpose:		Initializing LTC1864
	Input:			- Conversion start port
	Returns:		No
	************************************************************************/
	LTC1864(Port* CONV);
	
	/***********************************************************************
	Function:		GetResult()
	Purpose:		Initializing conversion and return result
	Input:			- Pointer to SPI_Master object					
	Returns:		Conversion result
	************************************************************************/
	uint16_t GetResult(SPI_Master* spiMaster);
	
	~LTC1864();
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	private:
	
	Port* conv;
};

#endif /* LTC1864_H_ */