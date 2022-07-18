#include "MatrixKeyboard.h"

MatrixKeyboard :: MatrixKeyboard(Port* R0_Port, Port* R1_Port, Port* R2_Port, Port* R3_Port, Port* R4_Port,
					 Port* C0_Port, Port* C1_Port, Port* C2_Port, Port* C3_Port)
{
	this->Rows[0] = R0_Port;
	this->Rows[1] = R1_Port;
	this->Rows[2] = R2_Port;
	this->Rows[3] = R3_Port;
	this->Rows[4] = R4_Port;
	
	this->Columns[0] = C0_Port;
	this->Columns[1] = C1_Port;
	this->Columns[2] = C2_Port;
	this->Columns[3] = C3_Port;
		
	onCounter = 0;
	offCounter = 0;
	
	contactFixed = false;
	
	tempPressedButton = ButtonsNum;
	pressedButton = ButtonsNum;
	
	for(uint8_t i = 0; i < RowsNum; i++)
	{
		this->Rows[i]->SetAsOutput();
	}	

	SetRows();
	
	for(uint8_t i = 0; i < ColumnsNum; i++)
	{
		this->Columns[i]->Set();
	}
}

void MatrixKeyboard :: SetRows()
{
	for(uint8_t i = 0; i < RowsNum; i++)
	{
		this->Rows[i]->Set();
	}
}

uint8_t MatrixKeyboard :: GetPressedButton(uint8_t row)
{
	uint8_t column;	
	bool columnSelected = false;
	
	for(uint8_t i = 0; i < ColumnsNum; i++)
	{
		if(this->Columns[i]->IsClear())
		{
			column = i;
			columnSelected = true;
			break;
		}
	}
	
	if(!columnSelected) return ButtonsNum;

	uint8_t pressedButton = column + (row * ColumnsNum);

	return pressedButton;
}

bool MatrixKeyboard :: ButtonIsPressed()
{
	for(uint8_t row = 0; row < RowsNum; row++)
	{
		this->Rows[row]->Clear();

		tempPressedButton = GetPressedButton(row);

		SetRows();

		if(tempPressedButton < ButtonsNum) break;
	}

	if(tempPressedButton < ButtonsNum)
	{
		if(!contactFixed)
		{
			if(tempPressedButton == pressedButton)
			{
				onCounter++;
				if(onCounter > ChecksNum)
				{
					contactFixed = true;
					return true;
				}
				return false;
			}
			pressedButton = tempPressedButton;
			onCounter = 0;
			return false;
		}
		return false;
	}

	else if(contactFixed && onCounter > ChecksNum)
	{
		if(offCounter > ChecksNum)
		{
			onCounter = 0;
			offCounter = 0;
			contactFixed = false;
			return false;
		}

		offCounter++;
		return false;
	}

	else return false;
}

uint8_t MatrixKeyboard :: GetPressedButton()
{		
	if(ButtonIsPressed()) return pressedButton;
	else return ButtonsNum;
}

MatrixKeyboard::~MatrixKeyboard()
{
	for(uint8_t i = 0; i < RowsNum; i++)
	{
		delete Rows[i];
	}
	
	for(uint8_t i = 0; i < ColumnsNum; i++)
	{
		delete Columns[i];
	}
}

void* MatrixKeyboard::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void MatrixKeyboard::operator delete(void* ptr)
{
	if(ptr != nullptr) free(ptr);
}


