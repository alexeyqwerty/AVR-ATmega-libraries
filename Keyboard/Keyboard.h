/************************************************************************************
Title:						Library for matrix keyboard handling 

Support:					- All AVR microcontrollers
		 
Notes:						- Rows is outputs, need the diodes connection
							- Columns is inputs
*************************************************************************************/
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <avr/io.h>

#include "Port.h"

/***********************************************************************
Description:		Number of rows and columns
************************************************************************/
#define RowsNum	5
#define ColumnsNum 4
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
	ENT, CLR, START, DIG0,
	ESC, DIG6, DIG3, RIGHT,
	DIG9, DIG5, DIG2, LEFT,
	DIG8, DIG4, DIG1, F2,
	DIG7, DOWN, UP, F1
};

class Keyboard
{
	public:	
	
	/***********************************************************************
	Function:		Keyboard()
	Purpose:		Initializing keyboard
	Input:			- Pointers to objects of the "Port" type for rows and
					  columns
	Returns:		No
	************************************************************************/
	Keyboard(Port* R0_Port, Port* R1_Port, Port* R2_Port, Port* R3_Port, Port* R4_Port,	
			 Port* C0_Port, Port* C1_Port, Port* C2_Port, Port* C3_Port);	
	
	/***********************************************************************
	Function:		GetPressedButton()
	Purpose:		Return number of pressed button
	Input:			No
	Returns:		Pressed button number
	************************************************************************/
	uint8_t GetPressedButton();
	
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
Keyboard keyboard(&R0_Port, &R1_Port ...,
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