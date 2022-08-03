/************************************************************************************
Title:						Port handling library 

Supported microcontrollers: All ATmega
		 
Notes:						- By default port is set as an input in the Z-state
*************************************************************************************/

#ifndef PORT_H_
#define PORT_H_

#include <avr/io.h>
#include <stdlib.h>

class Port
{
	public:
	
	/***********************************************************************
	Function:		Port()
	Purpose:		Initializing port
	Input:			- Port attributes (PORT, DDR, PIN and pin number)
	Returns:		No
	************************************************************************/
	Port(volatile uint8_t* PORT, volatile uint8_t* DDR, volatile uint8_t* PIN, uint8_t pinNum);

	/***********************************************************************
	Function:		SetAsInput()
	Purpose:		Setting port as input
	Input:			No
	Returns:		No
	************************************************************************/
	void SetAsInput();
	
	/***********************************************************************
	Function:		SetAsOutput()
	Purpose:		Setting port as output
	Input:			No
	Returns:		No
	************************************************************************/
	void SetAsOutput();

	/***********************************************************************
	Function:		Set()
	Purpose:		Setting port to the HIGH state
	Input:			No
	Returns:		No
	************************************************************************/
	void Set();

	/***********************************************************************
	Function:		Clear()
	Purpose:		Setting port to the LOW state
	Input:			No
	Returns:		No
	************************************************************************/	
	void Clear();
	
	/***********************************************************************
	Function:		IsSet()
	Purpose:		Return pin state
	Input:			No
	Returns:		Boolean value (true - if port in high state)
	************************************************************************/	
	bool IsSet();
	
	/***********************************************************************
	Function:		IsClear()
	Purpose:		Return pin state
	Input:			No
	Returns:		Boolean value (true - if port in low state)
	************************************************************************/	
	bool IsClear();
		
	~Port();

	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	private:
	
	volatile uint8_t* PORT;
	volatile uint8_t* DDR;
	volatile uint8_t* PIN;
	uint8_t pinNum;
};



#endif /* PORT_H_ */