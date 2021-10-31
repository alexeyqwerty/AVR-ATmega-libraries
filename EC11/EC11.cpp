#include "EC11.h"

void EC11::Init(Port* A, Port* B, Port* BTN /*= NULL*/)
{
	this->A = A;
	this->A->SetAsInput();
	this->A->Set();
	
	this->B = B;
	this->B->SetAsInput();
	this->B->Set();
	
	this->startRegistered = false;
	this->stopRegistered = false;

	this->direction = NO_DIR;
	
	if(BTN != NULL)
	{
		this->BTN = BTN;
		this->BTN->SetAsInput();
		this->BTN->Set();	
		
		this->buttonStation = true;
		this->contactFixed = false;
		this->onCounter = 0;
		this->offCounter = 0;
	}
	
}

EC11::EC11(Port* A, Port* B)
{
	Init(A, B);
}

EC11::EC11(Port* A, Port* B, Port* BTN)
{
	Init(A, B, BTN);
}

Direction EC11::GetDirection()
{
	if(this->startRegistered == true)
	{		
		if(this->A->IsClear() && this->B->IsClear())
		{
			this->startRegistered = false;
			this->stopRegistered = true;			
		}
	}
	
	else if(this->stopRegistered == true)
	{
		if(this->A->IsSet() && this->B->IsSet())
		{
			this->stopRegistered = false;
			return this->direction;
		}
	}
	
	else
	{
		if(this->A->IsClear() && this->B->IsSet())
		{
			this->startRegistered = true;				
			this->direction = LEFT_DIR;
		}
		
		if(this->B->IsClear() && this->A->IsSet())
		{
			this->startRegistered = true;
			this->direction = RIGHT_DIR;			
		}
	}
	
	return NO_DIR;
}

bool EC11::ButtonPressed()
{
	if(this->BTN == NULL) return false;
	
	bool buttonPressed = this->BTN->IsClear();
	
	if(!this->contactFixed && buttonPressed)
	{	
		if(this->buttonStation == buttonPressed)
		{
			if(++this->onCounter > numberOfChecks)
			{
				this->contactFixed = true;
				return true;
			}
		}
		else
		{
			this->buttonStation = buttonPressed;
			this->onCounter = 0;
		}		
	}
	
	else if(this->contactFixed && !buttonPressed)
	{
		if(++this->offCounter > numberOfChecks)
		{
			this->buttonStation = buttonPressed;
			this->onCounter = 0;
			this->offCounter = 0;
			this->contactFixed = false;
		}
	}
	
	return false;
}
