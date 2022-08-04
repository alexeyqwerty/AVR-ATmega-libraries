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

#define DATA_BITS 4

class HD44780
{
	public:
	
	/***********************************************************************
	Function:		HD44780()
	Purpose:		Initializing LCD
	Input:			- RS, RW, E, D4 ... D7 ports
					- Quantity of symbols in line (for different LCD's)
	Returns:		No
	************************************************************************/
	HD44780(Port* RS_Port, Port* RW_Port, Port* E_Port, 
			Port* D4_Port, Port* D5_Port,
			Port* D6_Port, Port* D7_Port,
			uint8_t symbolsInLine);
	
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
	
	~HD44780();
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	private:
		
	Port* RS_Port;
	Port* RW_Port;
	Port* E_Port;
	Port* DataPorts[DATA_BITS];
		
	uint8_t symbolsInLine;
	
	void SelectDevice();	
	
	void LatchData();
	
	void Wait();
	
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