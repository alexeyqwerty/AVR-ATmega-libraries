/************************************************************************************
Title:						16 bit DAC AD5541 library
		 
Notes:						- A pointer to SPI_Master object is used for set output
							  voltage
*************************************************************************************/

#ifndef AD5541_H_
#define AD5541_H_

#include <avr/io.h>
#include <stdlib.h>

#include "../SPI_Master/SPI_Master.h"
#include "../Port/Port.h"

class AD5541
{
	public:
	
	/***********************************************************************
	Function:		AD5541()
	Purpose:		Initializing AD5541
	Input:			- Chip select port 
	Returns:		No
	************************************************************************/
	AD5541(Port* CS);

	/***********************************************************************
	Function:		SetOutput()
	Purpose:		Set output voltage
	Input:			- Output voltage code
					- SPI master object
	Returns:		No
	************************************************************************/		
	void SetOutput(uint16_t data, SPI_Master* spiMaster);
	
	~AD5541();
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
		
	private:
		
	Port* CS;	
};

#endif /* AD5541_H_ */