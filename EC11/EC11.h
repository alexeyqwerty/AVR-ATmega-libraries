/************************************************************************************
Title:						Encoders EC11 handling library
		 
Notes:						- Supported encoders with- / without button
*************************************************************************************/

#ifndef EC11_H_
#define EC11_H_

#include "Port.h"

enum Direction
{
	NO_DIR,
	LEFT_DIR,
	RIGHT_DIR	
};

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
	Direction GetDirection();
	
	/***********************************************************************
	Function:		ButtonPressed()
	Purpose:		Returns button state
	Input:			No					
	Returns:		Boolean value (true - if button was pressed)
					Always returns false, if using without button
	************************************************************************/
	bool ButtonPressed();
	
	private:
	
	Port* A;
	Port* B;
	Port* BTN;
};

#endif /* EC11_H_ */

/************************************************************************
							HOW IT USE        							

					-=Creating the EC11 object=-

EC11 voltageRegulator(&V_REG_A, &V_REG_B, &V_REG_BTN);

*************************************************************************

  -=From time to time check the condition of the shaft and the button=-

switch(voltageRegulator.GetDirection())
{
	case LEFT_DIR: ExecuteVoltageRegulatorLeftDirection(); break;
	
	case RIGHT_DIR: ExecuteVoltageRegulatorRightDirection(); break;
	
	default: break;
}
	
if(voltageRegulator.ButtonPressed()) ExecuteVoltageRegulatorButtonPressed();			

*************************************************************************/