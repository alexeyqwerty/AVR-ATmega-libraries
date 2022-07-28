/************************************************************************************
Title:						Library for LCD with HD44780 controller using TWI 
							via PCF8574
Support:					- WH0802 series
							- WH1602 series
							- WH2004 series
							- etc.
		 
Notes:						- 4-bits interface mode
							- Cursor position starting with {0,0}
							- The BF (busy flag) is not used, time delays are used
							- To use delays need include util/delay.h library and
							  define F_CPU
							- TWI control using an 8-bit expander PCF8574
							- Cursor position control (if the cursor position is 
							  incorrect, printing will be canceled)
							- Text length control (text that goes off the screen will
							  not be displayed)
*************************************************************************************/

#ifndef __HD44780_H__
#define __HD44780_H__

#define F_CPU 8000000
#include <util/delay.h>

#include <stdlib.h>
#include <string.h>

#include "TWI.h"

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
Description:		Port numbers for connecting LCD to PCF8574
************************************************************************/
#define LCD_RS 0
#define LCD_RW 1
#define LCD_E 2
#define LCD_LED 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

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

/***********************************************************************
Description:		Buffer size for saving operations with LCD
************************************************************************/
#define LCD_BUF_SIZE 10

class HD44780_PCF8574 : public ITWI
{
	public:
	
	/***********************************************************************
	Function:		HD44780_PCF8574()
	Purpose:		Initializing LCD
	Input:			- TWI object pointer
					- TWI address
					- LCD type
	Returns:		No
	************************************************************************/	
	HD44780_PCF8574(TWI *twi, uint8_t A2, uint8_t A1, uint8_t A0, uint32_t clockFrequency, HD44780_Type displayType);

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
	Function:		ClearArea()
	Purpose:		Clearing some selected area in line
	Input:			- Area length to be cleared
					- Start position
					- Line number
	Returns:		No
	************************************************************************/
	void ClearArea(uint8_t length, uint8_t slot, uint8_t line);
	
	/***********************************************************************
	Function:		Print()
	Purpose:		Outputting some text in LCD
	Input:			- Pointer to text
					- Slot number
					- Line number
	Returns:		No
	************************************************************************/
	void Print(const char *text, uint8_t slot, uint8_t line);

	/***********************************************************************
	Function:		PrintAlignment()
	Purpose:		Outputting some text in LCD with alignment
	Input:			- Pointer to text					
					- Line number
					- Alignment side
	Returns:		No
	************************************************************************/
	void PrintAlignment(const char* text, uint8_t line, HD44780_Alignment alignment);
	
	/***********************************************************************
	Function:		InsertSymbol()
	Purpose:		Entering user characters in CGRAM
	Input:			- Symbol address in SGRAM, must be from 1 to 8
					- Pointer to symbol code array
	Returns:		No
	************************************************************************/
	void InsertSymbol(uint8_t TWI_Address, const char* symbol);
	
	~HD44780_PCF8574();
	
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
	
	protected:
	
	virtual TWI_TransactionStatus GetTransactionStatus() override;
	
	virtual uint32_t ClockFrequency() override;
	
	virtual void NextOperation(TWI_Status status) override;

	private:
	
	void ClearDataCounter();
	
	void PrepareBuffer(uint8_t dataSize);
	
	void SetCursorPosition(uint8_t slot, uint8_t line);	
	
	uint32_t FormByte(uint8_t data, HD44780_DataType dataType);	
	
	void UpdateWritePointer();
	
	void UpdateReadPointer();
	
	void AddCommand(uint8_t command, uint16_t delay);
	
	bool CursorOverside(uint8_t slot, uint8_t line);
	
	void NormaliseLength(uint8_t &length, uint8_t slot);
	
	TWI *twi;	
	uint8_t address;	
	uint32_t clockFrequency;	
	TWI_Status status;	
	TWI_TransactionStatus transactionStatus;
		
	uint8_t *buffer[LCD_BUF_SIZE];
	uint8_t dataSize[LCD_BUF_SIZE];
	uint16_t delay[LCD_BUF_SIZE];
	uint8_t dataCounter;
	uint8_t bufferCounter;
	uint8_t readPointer;
	uint8_t writePointer;	
	
	uint8_t symbols;
	uint8_t lines;
};

/***********************************************************************
								HOW IT USE
************************************************************************
							-=Create LCD object=-

1. Create TWI object
TWI *twi = new TWI(F_CPU);

2. Create LCD object
HD44780_PCF8574 *lcd = new HD44780_PCF8574(twi, 0, 0, 1, 50000, WH2004);

3.Add LCD in TWI
twi->AddDevice(lcd);
							
************************************************************************												
							-=Print some text=-
const char* someText = "123.56km to target";
						
To print some text use Print() or PrintAlignment(), where
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
To print this symbol use Print() or PrintAlignment(), 
where first argument - is RectangleSymbol
************************************************************************/

#endif //__HD44780_H__
