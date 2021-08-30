#include "FloatHandler.h"

float epsilon = 0.00001f;

bool FloatHandler::ValueChanged(float step, float oldValue, float newValue)
{
	float temp = fabs(newValue - oldValue);	

	return GreaterOrEquals(temp, step);
}

float FloatHandler::Round(float value, int8_t digitsAfterDot)
{
	uint32_t powPlus = (uint32_t)round(pow(10, digitsAfterDot));
	
	uint32_t roundedTemp = (uint32_t)round(value * powPlus);
	value = roundedTemp;
		
	return value / powPlus;
}

bool FloatHandler::Equals(float value_1, float value_2)
{
	return (fabs(value_1 - value_2) < epsilon);
}

bool FloatHandler::Less(float value_1, float value_2)
{
	return ((value_2 - value_1) > epsilon);
}

bool FloatHandler::LessOrEquals(float value_1, float value_2)
{	
	return Less(value_1, value_2) || Equals(value_1, value_2);
}

bool FloatHandler::Greater(float value_1, float value_2)
{
	return ((value_1 - value_2) > epsilon);
}

bool FloatHandler::GreaterOrEquals(float value_1, float value_2)
{	
	return Greater(value_1, value_2) || Equals(value_1, value_2);
}