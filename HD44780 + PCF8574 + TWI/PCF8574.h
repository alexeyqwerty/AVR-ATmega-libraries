/************************************************************************************
Title:						Library for 8-bit expander PCF8574 using I2C
*************************************************************************************/

#ifndef PCF8574_H_
#define PCF8574_H_

#include <avr/io.h>
#include "TWI.h"

class PCF8574
{
	public:
	/***********************************************************************
	Function:		PCF8574()
	Purpose:		Initializing expander
	Input:			- A0, A1, A2 - three bits of the address according to the 
					  connection scheme
					- TWI object pointer
	Returns:		No
	************************************************************************/
	PCF8574(uint8_t A0, uint8_t A1, uint8_t A2, TWI* twi);

	/***********************************************************************
	Function:		SetOutput()
	Purpose:		Sets the ports according to the data
	Input:			- Data
	Returns:		No
	************************************************************************/
	void SetOutput(uint8_t data);
	
	private:
	
	uint8_t address;	
	
	TWI* twi;
};
#endif /* PCF8574_H_ */