/***************************************************************************
Title:		Library for string handle
****************************************************************************/

#ifndef STRINGHANDLER_H_
#define STRINGHANDLER_H_

#include <stdint-gcc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

class StringHandler
{
	public:
	
	/***********************************************************************
	Function:		FloatToString()
	Purpose:		Convert float values to string format. Also possible to
					use integer values with null value of the number of digits
					after the decimal point
	Input:			Value for converting
					Number digits after dot
	Returns:		Constant char pointer to a converted string
	************************************************************************/
	static const char* FloatToString(float value, uint8_t digitsAfterDot);	
	
	/***********************************************************************
	Function:		Concat()
	Purpose:		Combining two strings
	Input:			First string
					Second string
	Returns:		Sum of two strings
	How use:		IT'S NECESSARY
					const char* text = StringHandler::Concat(s1, s2);
					.
					.	...do something with text...
					.
					free((char*)text);
					text = NULL;
	************************************************************************/	
	static const char* Concat(const char* string_1, const char* string_2);
	
	/***********************************************************************
	Function:		StartsWith()
	Purpose:		Determines whether the beginning of string_1 coincides
					with string_2. Returns false if the string_2 length greater
					string_1 length
	Input:			First string
					Second string
	Returns:		True - if the string_2 matches, false - if not	
	************************************************************************/	
	static bool StartsWith(const char* string_1, const char* string_2);
};

#endif /* HANDLEROFNUMBERS_H_ */