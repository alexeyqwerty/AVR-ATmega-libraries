#include "HD44780.h"

void HD44780::SelectDevice(uint8_t data)
{
	data |= (1<<E);	
	this->pcf8574->SetOutput(data);
	_delay_us(200);	
}

void HD44780::LatchData(uint8_t data)
{	
	data &= ~(1<<E);
	this->pcf8574->SetOutput(data);
	_delay_us(200);
}

void HD44780::SendByte(char data, DataType dataType)
{
	uint8_t _data = data;
	
	for(uint8_t i = 0; i < 2; i++)
	{
		_data |= (1<<LED);
		_data &= ~(1<<RW);
		
		if(dataType == COMMAND) _data &= ~(1<<RS);
		else if(dataType == DATA) _data |= (1<<RS);
		
		SelectDevice(_data);				
		
		LatchData(_data);
		
		_data = data << 4;
	}
}

void HD44780::SetCursorPosition(uint8_t slot, uint8_t line)
{
	if(line > 1)
	{
		line %= 2;
		slot += symbolsInLine;
	}
	
	uint8_t adress = 0x40 * line;
	adress += slot;
	adress |= 0x80;
	SendByte(adress, COMMAND);
	_delay_ms(2);
}

HD44780::HD44780(PCF8574* pcf8574, uint8_t symbolsInLine)
{		
	this->pcf8574 = pcf8574;
	this->symbolsInLine = symbolsInLine;	
	
	uint8_t data = (1<<D4)|(1<<D5)|(1<<LED);
	
	for(char i = 0; i < 2; i++)
	{		
 		SelectDevice(data);		
 		LatchData(data); 		
	}
	
	data = 0;
	data |= (1<<D5)|(1<<LED);
	
 	SelectDevice(data); 	
 	LatchData(data);
	
	SendByte(0b00101000, COMMAND);
	Off();
	Clear();
	SendByte(0b00000110, COMMAND);
	On();
}

void HD44780::On()
{
	SendByte(0b00001100, COMMAND);
}

void HD44780::Off()
{
	SendByte(0b00001000, COMMAND);
}

void HD44780::Clear()
{
	SendByte(0b00000001, COMMAND);
	_delay_ms(2);
}

void HD44780::ClearSelectedArea(uint8_t symbolsNum, uint8_t fromSlot, uint8_t line)
{
	SetCursorPosition(fromSlot, line);
	
	for(uint8_t i = 0; i < symbolsNum; i++) SendByte(' ', DATA);	
}

void HD44780::InsertSymbol(uint8_t address, const char* symbol)
{
	SendByte(0b01000000 | (address << 3), COMMAND);
	_delay_ms(2);
	
	for(uint8_t i = 0; i < 7; i++)
	{
		SendByte(symbol[i], DATA);
	}
}

void HD44780::PrintText(const char *text, uint8_t slot, uint8_t line)
{
	SetCursorPosition(slot, line);
			
	while( *text != '\0' )
	{		
		SendByte(*text++, DATA);		
	}		
}

void HD44780::PrintTextWithAlignment(const char* text, uint8_t line, Alignment alignment)
{
	uint8_t slot = 0;
	
	switch(alignment)
	{
		case LEFT_SIDE:
		{
			slot = 0;
		}break;
		
		case RIGHT_SIDE:
		{
			slot = symbolsInLine - strlen(text);
		}break;
		
		case CENTER_SCREEN:
		{
			slot = (symbolsInLine - strlen(text)) / 2;
		}break;
	}
	
	PrintText(text, slot, line);
}