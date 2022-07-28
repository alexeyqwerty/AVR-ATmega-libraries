/************************************************************************************
Title:						Library for matrix keyboard handling 

Support:					- All AVR microcontrollers
		 
Notes:						- Rows is outputs, need the diodes connection
							- Columns is inputs
*************************************************************************************/
#ifndef MATRIX_KEYBOARD_H
#define MATRIX_KEYBOARD_H

#include <avr/io.h>

#include "Port.h"

/***********************************************************************
Description:		Number of rows and columns
************************************************************************/
#define RowsNum	4
#define ColumnsNum 3
#define ButtonsNum (RowsNum * ColumnsNum)

/***********************************************************************
Description:		Number of checks before register of button click
************************************************************************/
#define ChecksNum 5

/***********************************************************************
Description:		Buttons layout according to the electrical connection
************************************************************************/
enum ButtonsLayout
{
	HASH, D0, STAR,
	D9, D8, D7,
	D6, D5, D4,
	D3, D2, D1
};

class MatrixKeyboard
{
	public:	
	
	/***********************************************************************
	Function:		MatrixKeyboard()
	Purpose:		Initializing keyboard
	Input:			- Pointers to objects of the "Port" type for rows and
					  columns
	Returns:		No
	************************************************************************/
	MatrixKeyboard(Port* R0_Port, Port* R1_Port, Port* R2_Port, Port* R3_Port,	
				   Port* C0_Port, Port* C1_Port, Port* C2_Port);	
	
	/***********************************************************************
	Function:		GetPressedButton()
	Purpose:		Return number of pressed button
	Input:			No
	Returns:		Pressed button number
	************************************************************************/
	uint8_t GetPressedButton();
	
	~MatrixKeyboard();
	
	void* operator new(size_t size);

	void operator delete(void* ptr);
	
	private:
	
	Port* Rows[RowsNum];

	Port* Columns[ColumnsNum];
	
	uint8_t onCounter, offCounter;
	
	bool contactFixed;
	
	uint8_t tempPressedButton, pressedButton;
	
	void SetRows();
	
	uint8_t GetPressedButton(uint8_t row);
	
	bool ButtonIsPressed();	
};

#endif

/***********************************************************************
								HOW IT USE
					
					-=Create the Keyboard object=-
MatrixKeyboard keyboard(&R0_Port, &R1_Port ...,
						&C0_Port, &C1_Port ...);
						
 -=From time to time check the condition of the shaft and the button=-

char button = keyboard.GetPressedButton();

switch((ButtonsLayout)button)
{
	case ENT: Execute_ENT(); break;
	case CLR: Execute_CLR(); break;
	
	...
	
	default: break;
}						

************************************************************************/