/************************************************************************************
Title:						12 bit ADC MCP3202 library
		 
Notes:						- One object has one input channel
							- A pointer to SPI_Master object is used for data exchange
*************************************************************************************/

#ifndef MCP3202_H_
#define MCP3202_H_

#include "Port.h"
#include "SPI_Master.h"

class MCP3202
{
	public:
	
	/***********************************************************************
	Function:		MCP3202()
	Purpose:		Initializing MCP3202
	Input:			- Chip select port
					- Channel number
	Returns:		No
	************************************************************************/
	MCP3202(Port* CS, uint8_t channel);

	/***********************************************************************
	Function:		GetResult()
	Purpose:		Initializing conversion and return result
	Input:			- Pointer to SPI_Master object					
	Returns:		Conversion result
	************************************************************************/
	uint16_t GetResult(SPI_Master* spiMaster);
	
	~MCP3202();
	
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
	
	private:
	
	uint8_t channel;
	
	Port* CS;
};

#endif /* MCP3202_H_ */