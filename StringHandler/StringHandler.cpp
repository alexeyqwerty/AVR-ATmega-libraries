#include "StringHandler.h"

const char* StringHandler::FloatToString(float value, uint8_t digitsAfterDot)
{
	uint32_t integerPart = fabs(value);
	
	uint8_t stringLength = 0;	
	do
	{
		integerPart /= 10;
		stringLength++;
	}
	while(integerPart > 0);
	
	integerPart = fabs(value);
	
	uint8_t integerPartLength = stringLength;
	
	if(digitsAfterDot > 0)
	{
		stringLength++;										//for '.'
		stringLength += digitsAfterDot;						//for .xxxxx
	}
	
	if(value < 0) stringLength++;							//for '-'
	
	stringLength++;											//for '\0'
			
	char tempString[stringLength + 1];		
			
	char integerPartDigitsArray[integerPartLength + 1];		// "+ 1" for '\0'
			
	for(uint8_t i = 1; i <= integerPartLength; i++)
	{
		uint8_t dig = integerPart % 10;
				
		integerPart /= 10;
				
		integerPartDigitsArray[integerPartLength - i] = dig + '0';
	}
	
	integerPartDigitsArray[integerPartLength] = '\0';
				
	const char* resultString = integerPartDigitsArray;
			
	if(value < 0)
	{
		strlcpy(tempString, "-", stringLength);
		strlcat(tempString, resultString, stringLength);
	}
	else strlcpy(tempString, resultString, stringLength);
	
	if(digitsAfterDot == 0)
	{		
		resultString = tempString;		
		return resultString;
	}
		
	strlcat(tempString, ".", stringLength);
		
	value = fabs(value) - integerPart;	
			
	uint32_t mult = (uint32_t)round(pow(10, digitsAfterDot));
	
	uint32_t fractionalPart = (uint32_t)round(value * mult);
			
	char fractionalPartDigitsArray[digitsAfterDot + 1];		//for '\0'
			
	for(uint8_t i = 1; i <= digitsAfterDot; i++)
	{
		uint8_t dig = fractionalPart % 10;
				
		fractionalPart /= 10;
				
		fractionalPartDigitsArray[digitsAfterDot - i] = dig + '0';
	}			
	fractionalPartDigitsArray[digitsAfterDot] = '\0';
	
	resultString = fractionalPartDigitsArray;
			
	strlcat(tempString, resultString, stringLength);
			
	resultString = tempString;
	
	return resultString;
}

const char* StringHandler::Concat(const char* string_1, const char* string_2)
{
	uint16_t str_1_Length = strlen(string_1);
	uint16_t str_2_Length = strlen(string_2);
	uint16_t stringLength = str_1_Length + str_2_Length + 1;
	
	char *concatenatedString = (char*)malloc(stringLength);
	
	strlcpy(concatenatedString, string_1, stringLength);
	strlcat(concatenatedString, string_2, stringLength);
	
	return concatenatedString;
}
