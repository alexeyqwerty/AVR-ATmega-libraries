/************************************************************************************
Title:						SPI Master library 

Supported microcontrollers: All ATmega
		 
Notes:						- The SS pin is set as output
							- By default transferring mode is set in mode 0
							  (polarity - rising, phase - start)
							- SCK frequency = F_CPU / 64
*************************************************************************************/

#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <avr/io.h>
#include <stdlib.h>
#include "Port.h"
#include "Devices.h"

enum SPI_Polarity
{
	SPI_RISING,
	SPI_FALLING	
};

enum SPI_Phase
{
	SPI_START,
	SPI_END
};

enum SPI_DataOrder
{
	SPI_MBF,
	SPI_LBF		
};

/***************************************************************************
				SPI modes
				
- Mode 0: CPOL -> RISING	CPHA -> START
- Mode 1: CPOL -> RISING	CPHA -> END
- Mode 2: CPOL -> FALLING	CPHA -> START
- Mode 1: CPOL -> FALLING	CPHA -> END
****************************************************************************/

class ISPI
{
	public:
	
	virtual SPI_Polarity Polarity();
	virtual SPI_Phase Phase();
	virtual SPI_DataOrder DataOrder();
	virtual uint8_t DataSize();
	virtual uint8_t *Data();
	virtual void Start();
	virtual void Finish();
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

	void AddDevice(ISPI *device);
	void NextOperation();
	void CheckDevices();
	
	~SPI_Master();
	
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
		
	private:
	
	bool spiBusy;
	
	Devices<ISPI> *devices;	
	
	uint8_t *data;
	uint8_t dataSize;
	uint8_t dataCounter;
};

#endif /* SPI_MASTER_H_ */