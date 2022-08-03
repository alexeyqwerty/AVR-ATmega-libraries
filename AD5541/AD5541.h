/************************************************************************************
Title:						16 bit DAC AD5541 library
		 
Notes:						- A pointer to SPI_Master object is used for set output
							  voltage
*************************************************************************************/

#ifndef AD5541_H_
#define AD5541_H_

#include <avr/io.h>
#include <stdlib.h>

#include "SPI_Master.h"
#include "Port.h"
#include "Buffer.h"

class AD5541 : public ISPI
{
	public:
	
	/***********************************************************************
	Function:		AD5541()
	Purpose:		Initializing AD5541
	Input:			- Chip select port 
	Returns:		No
	************************************************************************/
	AD5541(Port* CS, uint8_t bufferSize);

	/***********************************************************************
	Function:		SetOutput()
	Purpose:		Set output voltage
	Input:			- Output voltage code
					- SPI master object
	Returns:		No
	************************************************************************/		
	void SetOutput(uint16_t data);
	
	~AD5541();
	
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
		
	Port* CS;	
	
	Buffer *buffer;
};

#endif /* AD5541_H_ */