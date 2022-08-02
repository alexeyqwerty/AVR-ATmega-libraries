/************************************************************************************
Title:						Library for LCD with HD44780 controller 

Support:					- WH0802 series
							- WH1602 series
							- WH1604 series
							- WH2004 series
		 
Notes:						- 4-bits interface mode
							- Reading BF (busy flag) is used
							- Include "util/delay.h" to use delays to initialize the 
							  LCD display. F_CPU must also be defined
*************************************************************************************/

#ifndef HD44780_H
#define HD44780_H

#include <avr/io.h>

#include <stdlib.h>
#include <string.h>

#define F_CPU 8000000
#include <util/delay.h>

#include "Port.h"

/***********************************************************************
Description:		Types of display with HD44780 controller
************************************************************************/
enum HD44780_Type
{
	WH0802 = 802,
	WH1202 = 1202,
	WH1601 = 1601,
	WH1602 = 1602,
	WH1604 = 1604,
	WH2002 = 2002,
	WH2004 = 2004,
	WH2402 = 2402,
	WH4002 = 4002
};

/***********************************************************************
Description:		Alignment enumeration
************************************************************************/
enum HD44780_Alignment
{
	LEFT_SIDE,
	RIGHT_SIDE,
	CENTER_SCREEN
};

/***********************************************************************
Description:		Type of transmitted data
************************************************************************/
enum HD44780_DataType
{
	LCD_Data,
	LCD_Command
};

#define DATA_BITS 4

class HD44780
{
	public:
	
	/***********************************************************************
	Function:		HD44780()
	Purpose:		Initializing LCD
	Input:			- RS, RW, E, D4 ... D7 ports
					- LCD type
	Returns:		No
	************************************************************************/
	HD44780(Port* RS_Port, Port* RW_Port, Port* E_Port, 
			Port* D4_Port, Port* D5_Port,
			Port* D6_Port, Port* D7_Port,
			HD44780_Type displayType);
	
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
	void ClearArea(uint8_t length, uint8_t slot, uint8_t line);

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
	void Print(const char *text, uint8_t slot, uint8_t line);

	/***********************************************************************
	Function:		PrintTextWithAlignment()
	Purpose:		Outputting some text in LCD with alignment
	Input:			- Pointer to text					
					- Line number
					- Alignment side
	Returns:		No
	************************************************************************/
	void PrintAlignment(const char* text, uint8_t line, HD44780_Alignment alignment);	
	
	~HD44780();
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	private:
		
	Port* RS_Port;
	Port* RW_Port;
	Port* E_Port;
	Port* DataPorts[DATA_BITS];
		
	uint8_t symbols;
	uint8_t lines;
	
	void SelectDevice();	
	
	void LatchData();
	
	void Wait();
	
	void SendByte(char data, HD44780_DataType dataType);
	
	void SetCursorPosition(uint8_t slot, uint8_t line);	
	
	bool CursorOverside(uint8_t slot, uint8_t line);
	
	void NormaliseLength(uint8_t &length, uint8_t slot);
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