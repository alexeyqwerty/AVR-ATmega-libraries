/************************************************************************************
Title:						12 bit DAC MCP4921 library
		 
Notes:						- A pointer to SPI_Master object is used for set output
							  voltage
*************************************************************************************/

#ifndef MCP4921_H
#define MCP4921_H

#include <avr/io.h>

#include "Port.h"
#include "SPI_Master.h"
#include "Buffer.h"

class MCP4921 : public ISPI
{
	public:
	
	/***********************************************************************
	Function:		MCP4921()
	Purpose:		Initializing MCP4921
	Input:			- Chip select port 
					- Latch data port
	Returns:		No
	************************************************************************/
	MCP4921(Port *CS, Port *LDAC, uint8_t bufferSize);
	
	/***********************************************************************
	Function:		SetOutput()
	Purpose:		Set output voltage
	Input:			- Output voltage code
					- SPI master object
	Returns:		No
	************************************************************************/	
	void SetOutput(uint16_t data);	
	
	~MCP4921();
	
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
	
	Port *CS;
	Port *LDAC;
	
	Buffer *buffer;
};

#endif 