/************************************************************************************
Title:						Library for LCD with HD44780 controller using I2C 
							via PCF8574

Support:					- WH0802 series
							- WH1602 series
							- WH1604 series
							- WH2004 series
		 
Notes:						- 4-bits interface mode
							- The BF (busy flag) is not used, time delays are used
							- To use delays need include util/delay.h library and
							  define F_CPU
							- I2C control using an 8-bit expander PCF8574
*************************************************************************************/

#ifndef HD44780_H
#define HD44780_H

#include <stdlib.h>
#include <string.h>

#define F_CPU 8000000
#include <util/delay.h>

#include "PCF8574.h"

/***********************************************************************
Description:		Port numbers for connecting LCD
************************************************************************/
#define RS 0
#define RW 1
#define E 2
#define LED 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

/***********************************************************************
Description:		Alignment enumeration
************************************************************************/
enum Alignment
{
	LEFT,
	RIGHT,
	CENTER	
};

/***********************************************************************
Description:		Type of transmitted data
************************************************************************/
enum DataType
{
	DATA,
	COMMAND
};

class HD44780
{
	public:
	
	/***********************************************************************
	Function:		HD44780()
	Purpose:		Initializing LCD
	Input:			- PCF8574 object pointer
					- Quantity of symbols in line (for different LCD's)
	Returns:		No
	************************************************************************/
	HD44780(PCF8574* pcf8574, uint8_t symbolsInLine);
	
	/***********************************************************************
	Functions:		On()
					Off()
	Purpose:		On and off LCD
	Input:			No
	Returns:		No
	************************************************************************/
	void On();	
	void Off();
	
	/***********************************************************************
	Function:		Clear()
	Purpose:		Clear LCD
	Input:			No
	Returns:		No
	************************************************************************/
	void Clear();
	
	/***********************************************************************
	Function:		ClearSelectedArea()
	Purpose:		Clearing some selected area in line
	Input:			- The number of symbols to be cleared
					- Start position
					- Line number
	Returns:		No
	************************************************************************/	
	void ClearSelectedArea(uint8_t symbolsNum, uint8_t fromSlot, uint8_t line);

	/***********************************************************************
	Function:		InsertSymbol()
	Purpose:		Entering user characters in CGRAM
	Input:			- Symbol address in SGRAM, must be from 1 to 8
					- Pointer to symbol code array
	Returns:		No
	************************************************************************/
	void InsertSymbol(uint8_t address, const char* symbol);	

	/***********************************************************************
	Function:		PrintText()
	Purpose:		Outputting some text in LCD
	Input:			- Pointer to text
					- Slot number
					- Line number
	Returns:		No
	************************************************************************/
	void PrintText(const char *text, uint8_t slot, uint8_t line);

	/***********************************************************************
	Function:		PrintTextWithAlignment()
	Purpose:		Outputting some text in LCD with alignment
	Input:			- Pointer to text					
					- Line number
					- Alignment side
	Returns:		No
	************************************************************************/
	void PrintTextWithAlignment(const char* text, uint8_t line, Alignment alignment);	
	
	private:
		
	PCF8574* pcf8574;
		
	uint8_t symbolsInLine;
	
	void SelectDevice();	
	
	void LatchData(uint8_t data);
	
	void SendByte(char data, DataType dataType);
	
	void SetCursorPosition(uint8_t slot, uint8_t line);	
};

#endif

/***********************************************************************
								HOW IT USE
						
							-=Print some text=-

const char* someText = "123.56km to target";
						
To print some text use PrintText() or PrintTextWithAlignment(), where
first argument is someText
						
************************************************************************						
							-=Create user symbol=-
						
For example:
------------
We need rectangle, like this:	* * * * *
								*       *
								*       *
								*       *
								*       *
								*       *
								* * * * *
								
First, need to create an array (7 elements) with the symbol code, 
where 1 - is shaded area and 0 - is empty area:

#define Rectangle (const char[]){0b00011111, 
								 0b00010001, 
								 0b00010001, 
								 0b00010001, 
								 0b00010001, 
								 0b00010001, 
								 0b00011111}
								 
Next, use InsertSymbol(_some_addres_, Rectangle);

Create a string representation:

#define RectangleSymbol (const char[]){_selected_address_, '\0'}

To print this symbol use PrintText() or PrintTextWithAlignment(), 
where first argument - is RectangleSymbol
************************************************************************/