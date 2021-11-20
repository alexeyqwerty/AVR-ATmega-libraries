#include "HD44780.h"

void HD44780::SelectDevice()
{
	this->E_Port->Set();	
}

void HD44780::LatchData()
{	
	this->E_Port->Clear();
}

void HD44780::Wait()
{
	for(uint8_t i = 0; i < DATA_BITS; i++)
	{
		this->DataPorts[i]->Set();
		this->DataPorts[i]->SetAsInput();		
	}
		
	this->RW_Port->Set();
	this->RS_Port->Clear();	
	
	bool LCD_Busy = false;
	
	do 
	{
		SelectDevice();
		
		LCD_Busy = this->DataPorts[3]->IsSet();
	
		LatchData();		
		SelectDevice();		
		LatchData();		
		
	} while (LCD_Busy);
	
	for(uint8_t i = 0; i < DATA_BITS; i++)
	{
		this->DataPorts[i]->SetAsOutput();
		this->DataPorts[i]->Clear();
	}
	
	this->RW_Port->Clear();
}

void HD44780::SendByte(char data, DataType dataType)
{	
	Wait();
	
	if(dataType == COMMAND) this->RS_Port->Clear();
	else if(dataType == DATA) this->RS_Port->Set();
	
	uint8_t _data = data >> DATA_BITS;
	
	for(uint8_t i = 0; i < 2; i++)
	{		
		SelectDevice();				
				
		for(uint8_t j = 0; j < DATA_BITS; j++)
		{
			if(_data & (1<<j)) this->DataPorts[j]->Set();
			else this->DataPorts[j]->Clear();
		}
			
		LatchData();
		
		_data = data;		
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
}

HD44780::HD44780(Port* RS_Port, Port* RW_Port, Port* E_Port,
				 Port* D4_Port, Port* D5_Port,
				 Port* D6_Port, Port* D7_Port,
				 uint8_t symbolsInLine)
{	
	this->symbolsInLine = symbolsInLine;	
	
	this->RS_Port = RS_Port;
	this->RW_Port = RW_Port;
	this->E_Port = E_Port;
	this->DataPorts[0] = D4_Port;
	this->DataPorts[1] = D5_Port;
	this->DataPorts[2] = D6_Port;
	this->DataPorts[3] = D7_Port;
	
	this->RS_Port->SetAsOutput();
	this->RW_Port->SetAsOutput();
	this->E_Port->SetAsOutput();
	
	this->RW_Port->Clear();
	
	for(uint8_t i = 0; i < DATA_BITS; i++)
	{
		this->DataPorts[i]->SetAsOutput();
		this->DataPorts[i]->Clear();
	}

	this->DataPorts[0]->Set();
	this->DataPorts[1]->Set();
	
	
	for(char i = 0; i < 2; i++)
	{		
 		SelectDevice(); 	
		_delay_us(2000);	 
 		LatchData(); 		
		_delay_us(2000);
	}
	
	this->DataPorts[0]->Clear();
	
 	SelectDevice(); 	
	_delay_us(2000);	
 	LatchData();
	_delay_us(2000);
	
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
	//_delay_ms(2);
}

void HD44780::ClearSelectedArea(uint8_t symbolsNum, uint8_t fromSlot, uint8_t line)
{
	SetCursorPosition(fromSlot, line);
	
	for(uint8_t i = 0; i < symbolsNum; i++) SendByte(' ', DATA);	
}

void HD44780::InsertSymbol(uint8_t address, const char* symbol)
{
	SendByte(0b01000000 | (address << 3), COMMAND);
	//_delay_ms(2);
	
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
		case LEFT:
		{
			slot = 0;
		}break;
		
		case RIGHT:
		{
			slot = symbolsInLine - strlen(text);
		}break;
		
		case CENTER:
		{
			slot = (symbolsInLine - strlen(text)) / 2;
		}break;
	}
	
	PrintText(text, slot, line);
}