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
	this->leftTimes = 0;
	this->rightTimes = 0;
	
	if(BTN != nullptr)
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

void EC11::RegisterDirection()
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
			
			switch(this->direction)
			{
				case NO_DIR: return;
				
				case LEFT_DIR:
				{
					this->leftTimes++;
					
					if(rightTimes > 0) rightTimes--;					
				}break;
				
				case RIGHT_DIR:
				{
					this->rightTimes++;
					
					if(leftTimes > 0) leftTimes--;
				}break;
			}
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
}

uint8_t EC11::GetLeftTimes()
{
	uint8_t _leftTimes = this->leftTimes;
	
	this->leftTimes = 0;
	
	return _leftTimes;
}

uint8_t EC11::GetRightTimes()
{
	uint8_t _rightTimes = this->rightTimes;
	
	this->rightTimes = 0;
	
	return _rightTimes;
}

bool EC11::ButtonPressed()
{
	if(this->BTN == nullptr) return false;
	
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

EC11::~EC11()
{
	delete this->A;
	delete this->B;
	delete this->BTN;
}

void* EC11::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void EC11::operator delete(void* ptr)
{
	if(ptr != nullptr) free(ptr);
}
