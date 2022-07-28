#ifndef MCP4921_H
#define MCP4921_H

#include <avr/io.h>

#include "Port.h"
#include "SPI_Master.h"

class MCP4921
{
	public:
	
	MCP4921(Port *CS, Port *LDAC);
	
	void SetOutput(uint16_t data, SPI_Master* spiMaster);	
	
	~MCP4921();
	
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
		
	private:
	
	Port *CS;
	Port *LDAC;
};

#endif 