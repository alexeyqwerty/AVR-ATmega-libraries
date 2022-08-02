/************************************************************************************
Title:						12 bit ADC MCP3202 library
		 
Notes:						- One object has one input channel
							- A pointer to SPI_Master object is used for data exchange
*************************************************************************************/

#ifndef MCP3202_H_
#define MCP3202_H_

#include "Port.h"
#include "SPI_Master.h"
#include "Buffer.h"

class MCP3202 : public ISPI
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
	uint16_t GetResult();
	
	~MCP3202();
	
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
	
	uint8_t channel;
	
	Port* CS;
	
	Buffer *buffer;	
	
	uint16_t result;
	
	void PrepareDataChange();
};

#endif /* MCP3202_H_ */