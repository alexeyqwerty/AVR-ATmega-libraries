/************************************************************************************
Title:						16 bit ADC LTC1864 library
		 
Notes:						- A pointer to SPI_Master object is used for data exchange
*************************************************************************************/

#ifndef LTC1864_H_
#define LTC1864_H_

#include <avr/io.h>
#include <stdlib.h>

#include "SPI_Master.h"
#include "Port.h"
#include "Buffer.h"

class LTC1864 : public ISPI
{
	public:
	
	/***********************************************************************
	Function:		LTC1864()
	Purpose:		Initializing LTC1864
	Input:			- Conversion start port
	Returns:		No
	************************************************************************/
	LTC1864(Port* CS);
	
	/***********************************************************************
	Function:		GetResult()
	Purpose:		Initializing conversion and return result
	Input:			- Pointer to SPI_Master object					
	Returns:		Conversion result
	************************************************************************/
	uint16_t GetResult();
	
	~LTC1864();
	
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
	
	uint16_t result;
	
	void PrepareDataChange();
};

#endif /* LTC1864_H_ */