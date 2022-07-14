/************************************************************************************
Title:						Encoders EC11 handling library
		 
Notes:						- Supported encoders with- / without button
*************************************************************************************/

#ifndef EC11_H_
#define EC11_H_

#include <stdlib.h>
#include "../Port/Port.h"

enum Direction
{
	NO_DIR,
	LEFT_DIR,
	RIGHT_DIR	
};

#define numberOfChecks 10

class EC11
{
	public:
	
	/***********************************************************************
	Functions:		EC11()
	Purpose:		Initializing EC11 with- / without button
	Input:			- A- B- ports
					- BTN port
	Returns:		No
	************************************************************************/
	EC11(Port* A, Port* B);
	EC11(Port* A, Port* B, Port* BTN);
		 
	/***********************************************************************
	Function:		GetDirection()
	Purpose:		Returns the direction of rotation of the shaft
	Input:			No					
	Returns:		Direction value
	************************************************************************/
	void RegisterDirection();
	
	uint8_t GetLeftTimes();
	
	uint8_t GetRightTimes();
	
	/***********************************************************************
	Function:		ButtonPressed()
	Purpose:		Returns button state
	Input:			No					
	Returns:		Boolean value (true - if button was pressed)
					Always returns false, if using without button
	************************************************************************/
	bool ButtonPressed();
	
	~EC11();
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	private:
	
	void Init(Port* A, Port* B, Port* BTN = NULL);
	
	Port* A;
	Port* B;
	Port* BTN;
	
	bool startRegistered;
	bool stopRegistered;

	Direction direction;
	uint8_t leftTimes;
	uint8_t rightTimes;

	bool buttonStation;
	bool contactFixed;
	uint8_t onCounter;
	uint8_t offCounter;
};

#endif /* EC11_H_ */

/************************************************************************
							HOW IT USE        							

Creating the control port objects and EC11 object:

Port *portA = new Port(...);

...

EC11 *encoder = new EC11(portA, portB, portBtn);

*************************************************************************

From time to time, register the direction of the shaft and poll the status 
of the button (make this call every 1 ... 3 ms):

void CheckEncoder()
{
	encoder->RegisterDirection();

	if(encoder->ButtonPressed()) HandlePressedButton();	
}

Check the registered shaft revolutions from time to time
(make this call every 100 ... 500 ms):

void HandleEncoder()
{
	uint8_t leftTimes = voltageChangeEncoder->GetLeftTimes();
	uint8_t rightTimes = voltageChangeEncoder->GetRightTimes();
	
	if(leftTimes > 0)
	{
		...make some
	}
	
	else if(rightTimes > 0)
	{
		...make some
	}
}	

*The functions Get***Times() reset the registered shaft revolutions

*************************************************************************/