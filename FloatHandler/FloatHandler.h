/************************************************************************************
Title:						Library for handling floating-point values				
*************************************************************************************/

#ifndef FLOATHANDLER_H_
#define FLOATHANDLER_H_

#include <math.h>
#include <stdint-gcc.h>

class FloatHandler
{
	public:
	
	/***********************************************************************
	Function:		ValueChanged()
	Purpose:		Checking the value change for some step value
	Input:			Change step
					Old value
					New value
	Returns:		Boolean value. True - if changed
	************************************************************************/
	static bool ValueChanged(float step, float oldValue, float newValue);
	
	/***********************************************************************
	Function:		Round()
	Purpose:		Rounding a floating-point value with a specified accuracy
	Input:			Value for rounding
					The number of digits after the dot					
	Returns:		Rounded value
	************************************************************************/
	static float Round(float value, int8_t digitsAfterDot);
	
	/***********************************************************************
	Functions:		Equals()
					Less()
					LessOrEquals()
					Greater()
					GreaterOrEquals()
	Purpose:		Return the relation between two floating-point numbers
	Input:			Value-source for comparison
					Compared value									
	Returns:		Boolean value. True - if the compared value corresponds 
					to the function name
	************************************************************************/
	static bool Equals(float value_1, float value_2);	
	static bool Less(float value_1, float value_2);	
	static bool LessOrEquals(float value_1, float value_2);	
	static bool Greater(float value_1, float value_2);	
	static bool GreaterOrEquals(float value_1, float value_2);
};

#endif /* HANDLEROFNUMBERS_H_ */