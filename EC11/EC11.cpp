#include "EC11.h"

bool startRegistered = false;
bool stopRegistered = false;

Direction direction = NO_DIR;

bool buttonStation = true;
bool contactFixed = false;
uint8_t onCounter = 0;
uint8_t offCounter = 0;
const uint8_t numberOfChecks = 10;

EC11::EC11(Port* A, Port* B)
{
	this->A = A;
	this->A->SetAsInput();
	this->A->Set();
	
	this->B = B;
	this->B->SetAsInput();
	this->B->Set();
}

EC11::EC11(Port* A, Port* B, Port* BTN)
{
	EC11(A, B);
	
	this->BTN = BTN;
	this->BTN->SetAsInput();
	this->BTN->Set();	
}

Direction EC11::GetDirection()
{
	if(startRegistered == true)
	{		
		if(this->A->IsClear() && this->B->IsClear())
		{
			startRegistered = false;
			stopRegistered = true;			
		}
	}
	
	else if(stopRegistered == true)
	{
		if(this->A->IsSet() && this->B->IsSet())
		{
			stopRegistered = false;
			return direction;
		}
	}
	
	else
	{
		if(this->A->IsClear() && this->B->IsSet())
		{
			startRegistered = true;				
			direction = LEFT_DIR;
		}
		
		if(this->B->IsClear() && this->A->IsSet())
		{
			startRegistered = true;
			direction = RIGHT_DIR;			
		}
	}
	
	return NO_DIR;
}

bool EC11::ButtonPressed()
{
	if(this->BTN == NULL) return false;
	
	bool buttonPressed = this->BTN->IsClear();
	
	if(buttonPressed)
	{		
		if(!contactFixed)
		{
			if(buttonStation == buttonPressed)
			{
				if(++onCounter > numberOfChecks)
				{
					contactFixed = true;
					return true;
				}
			}
			else
			{
				buttonStation = buttonPressed;
				onCounter = 0;
			}
		}
	}
	
	else if(contactFixed && !buttonPressed)
	{
		if(++offCounter > numberOfChecks)
		{
			buttonStation = buttonPressed;
			onCounter = 0;
			offCounter = 0;
			contactFixed = false;
		}
	}
	
	return false;
}
