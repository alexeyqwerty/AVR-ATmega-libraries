/************************************************************************************
Title:						Library for TWI (I2C) with using interrupts

Support:					All ATmega- microcontrollers

Notes:						- supports up to 128 devices
							- all device classes must implement the ITWI interface
							- only for the master mode
*************************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <stdlib.h>

/************************************************************************************
Description:		ITWI devices quantity (max 128)
*************************************************************************************/
#define TWI_DEVICES 3

/************************************************************************************
Description:		TWI status codes
*************************************************************************************/
enum TWI_Status
{
	TWI_Start = 0x08,						//A START condition has been transmitted
	TWI_Restart = 0x10,						//A repeated START condition has been transmitted
	TWI_ArbitrationLost = 0x38,				//Arbitration lost
	TWI_MasterAddressWriteACK = 0x18,		//SLA+W has been transmitted, ACK has been received
	TWI_MasterAddressWriteNACK = 0x20,		//SLA+W has been transmitted, NOT ACK has been received
	TWI_MasterDataWriteACK = 0x28,			//Data byte has been transmitted, ACK has been received
	TWI_MasterDataWriteNACK = 0x30,			//Data byte has been transmitted, NOT ACK has been received
	TWI_MasterAddressReadACK = 0x40,		//SLA+R has been transmitted, ACK has been received
	TWI_MasterAddressReadNACK = 0x48,		//SLA+R has been transmitted, NOT ACK has been received
	TWI_MasterDataReadACK = 0x50,			//Data byte has been received, ACK has been received
	TWI_MasterDataReadNACK = 0x58,			//Data byte has been received, NOT ACK has been received
	TWI_NoActions = 0xF8,					//No relevant state information	available, TWINT = “0”
	TWI_BusError = 0x00						//Bus error due to an illegal START or STOP condition
};

/************************************************************************************
Description:		TWI operations
*************************************************************************************/
enum TWI_Operation
{
	TWI_NoOperation,	
	TWI_Write,
	TWI_Read
};

/************************************************************************************
Description:		TWI transaction statuses
*************************************************************************************/
enum TWI_TransactionStatus
{
	TWI_Ready,
	TWI_InProcess,
	TWI_Ok,
	TWI_Error
};


/************************************************************************************
Description:		TWI device interface
*************************************************************************************/
class ITWI
{
	public:
	
	virtual uint32_t ClockFrequency() = 0;
	virtual TWI_TransactionStatus GetTransactionStatus() = 0;
	virtual void NextOperation(TWI_Status status) = 0;
};

class TWI
{
	public:
	
	/***********************************************************************
	Function:		TWI()
	Purpose:		Initializing TWI
	Input:			- Processor tact frequency value
	Returns:		No
	************************************************************************/
	TWI(uint32_t processorFrequency);
		
	/***********************************************************************
	Function:		AddDevice()
	Purpose:		Adding devices to the TWI bus
	Input:			- A device pointer that implements the ITWI interface
	Returns:		No
	************************************************************************/
	void AddDevice(ITWI *device);
	
	/***********************************************************************
	Function:		CheckDevices()
	Purpose:		Alternate polling of added devices for the presence of
					a data exchange request
	Input:			No
	Returns:		No
	************************************************************************/
	void CheckDevices();

	/***********************************************************************
	Function:		HandleDataChange()
	Purpose:		Saving the TWI status in the current device
	Input:			No
	Returns:		No
	************************************************************************/	
	void HandleDataChange();	
	
	/***********************************************************************
	Function:		Start()
	Purpose:		Initiate a START condition on the TWI bus
	Input:			No
	Returns:		No
	************************************************************************/
	void Start();

	/***********************************************************************
	Function:		WriteToTWDR()
	Purpose:		Write a data byte to the TWI data register
	Input:			- Data
	Returns:		No
	************************************************************************/	
	void WriteToTWDR(uint8_t data);

	/***********************************************************************
	Function:		ACK()
	Purpose:		Initiate a ACK condition on the TWI bus
	Input:			No
	Returns:		No
	************************************************************************/
	void ACK();
	
	/***********************************************************************
	Function:		NACK()
	Purpose:		Initiate a NACK condition on the TWI bus
	Input:			No
	Returns:		No
	************************************************************************/
	void NACK();
	
	/***********************************************************************
	Function:		ReadFromTWDR()
	Purpose:		Read a data byte from TWI data register
	Input:			No
	Returns:		- Data
	************************************************************************/		
	uint8_t ReadFromTWDR();
	
	/***********************************************************************
	Function:		Stop()
	Purpose:		Initiate a STOP condition on the TWI bus
	Input:			No
	Returns:		No
	************************************************************************/
	void Stop();

	/***********************************************************************
	Function:		DisableInterrupt()
	Purpose:		Disabling the TWI interrupt
	Input:			No
	Returns:		No
	************************************************************************/	
	void DisableInterrupt();
	
	~TWI();
		
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
	
	private:
	
	void SetClockFrequency(uint32_t clockFrequency);
	
	uint32_t processorFrequency;
	
	uint32_t clockFrequency;
	
	ITWI *devices[TWI_DEVICES];
	
	uint8_t devicesQuantiy;
	uint8_t selectedDevice;
};

/***********************************************************************
								HOW IT USE
						
- First need create TWI object:

	TWI *twi = new TWI(F_CPU);

- Next, need create object(s) which implement ITWI interface and
  include twi pointer:

	SomeITWI *someITWI = new ITWI(twi, ...);
	...

- Add to twi object created objects:

	twi->AddDevice(someITWI);
	
- From time to call CheckDevices method:
	
	twi->CheckDevices();
	
- In TWI interrupt call HandleDataChange() method:

	ISR(TWI_vect)
	{
		twi->HandleDataChange();
	}
				
************************************************************************/

#endif