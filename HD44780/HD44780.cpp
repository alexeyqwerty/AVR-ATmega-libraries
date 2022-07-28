#include "HD44780.h"

HD44780::HD44780(Port* RS_Port, Port* RW_Port, Port* E_Port,
Port* D4_Port, Port* D5_Port,
Port* D6_Port, Port* D7_Port,
HD44780_Type displayType)
{
	this->symbols = (uint16_t)displayType / 100;
	this->lines = (uint16_t)displayType % 100;
	
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
	
	
	for(uint8_t i = 0; i < 2; i++)
	{
		SelectDevice();		
		LatchData();
		_delay_us(5000);
	}
	
	this->DataPorts[0]->Clear();
	
	SelectDevice();
	LatchData();
	_delay_us(100);
	
	SendByte(0b00101000, LCD_Command);
	Off();
	Clear();
	SendByte(0b00000110, LCD_Command);
	On();
}

void HD44780::On()
{
	SendByte(0b00001100, LCD_Command);
}

void HD44780::Off()
{
	SendByte(0b00001000, LCD_Command);
}

void HD44780::Clear()
{
	SendByte(0b00000001, LCD_Command);
}

void HD44780::ClearArea(uint8_t length, uint8_t slot, uint8_t line)
{
	if(CursorOverside(slot, line)) return;
	SetCursorPosition(slot, line);	
	
	NormaliseLength(length, slot);
	for(uint8_t i = 0; i < length; i++) SendByte(' ', LCD_Data);
}

void HD44780::InsertSymbol(uint8_t address, const char* symbol)
{
	SendByte(0b01000000 | (address << 3), LCD_Command);	
	
	for(uint8_t i = 0; i < 7; i++)
	{
		SendByte(symbol[i], LCD_Data);
	}
}

void HD44780::Print(const char *text, uint8_t slot, uint8_t line)
{
	if(CursorOverside(slot, line)) return;
	SetCursorPosition(slot, line);
	
	uint8_t length = strlen(text);
	NormaliseLength(length, slot);
	for(uint8_t i = 0; i < length; i++)
	{
		SendByte(*text++, LCD_Data);
	}
}

void HD44780::PrintAlignment(const char* text, uint8_t line, HD44780_Alignment alignment)
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
			slot = this->symbols - strlen(text);
		}break;
		
		case CENTER_SCREEN:
		{
			slot = (this->symbols - strlen(text)) / 2;
		}break;
	}
	
	Print(text, slot, line);
}

HD44780::~HD44780(){}

void* HD44780::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void HD44780::operator delete(void* ptr)
{
	free(ptr);
}

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

void HD44780::SendByte(char data, HD44780_DataType dataType)
{	
	Wait();
	
	if(dataType == LCD_Command) this->RS_Port->Clear();
	else if(dataType == LCD_Data) this->RS_Port->Set();
	
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
		slot += this->symbols;
	}
	
	uint8_t adress = 0x40 * line;
	adress += slot;
	adress |= 0x80;
	SendByte(adress, LCD_Command);
}

bool HD44780::CursorOverside(uint8_t slot, uint8_t line)
{
	return (slot >= this->symbols) || (line >= this->lines);
}

void HD44780::NormaliseLength(uint8_t &length, uint8_t slot)
{
	uint8_t *_length = (uint8_t *)&length;
	*_length = slot + *_length <= this->symbols ? *_length : this->symbols - slot;
}
