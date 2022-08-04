#include "HD44780_PCF8574.h"

HD44780_PCF8574::HD44780_PCF8574(TWI *twi, uint8_t A2, uint8_t A1, uint8_t A0, HD44780_Type displayType)
{
	this->twi = twi;
		
	this->address = 0b01000000 | (A2 << 3) | (A1 << 2) | (A0 << 1);
		
	this->status = TWI_NoActions;
		
	this->symbols = (uint16_t)displayType / 100;
	this->lines = (uint16_t)displayType % 100;
	
	ClearDataCounter();
	
	this->bufferCounter = 0;
	
	this->readPointer = 0;
	
	this->writePointer = 0;
	
 	AddCommand(0b00110011, 5000);
 	AddCommand(0b00110010, 100);
 	AddCommand(0b00101000, 50);
 	Off();
 	Clear();
 	AddCommand(0b00000110, 50);
 	On();
}

void HD44780_PCF8574::On()
{
	AddCommand(0b00001100, 50);
}

void HD44780_PCF8574::Off()
{
	AddCommand(0b00001000, 50);
}

void HD44780_PCF8574::Clear()
{
	AddCommand(0b00000001, 2000);
}

void HD44780_PCF8574::ClearArea(uint8_t length, uint8_t slot, uint8_t line)
{
	if(CursorOverside(slot, line)) return;	
	SetCursorPosition(slot, line);
	
	while(bufferCounter == LCD_BUF_SIZE);	
	
	NormaliseLength(length, slot);
	PrepareBuffer(length * sizeof(uint32_t));	
	
	uint32_t _data = FormByte(' ', LCD_Data);
	
	for(uint8_t i = 0; i < this->dataSize[this->writePointer]; i += sizeof(uint32_t))
	{				
		memcpy(buffer[this->writePointer] + i, (uint8_t*)&_data, sizeof(uint32_t));
	}
	
	this->delay[this->writePointer] = 50;
	
	UpdateWritePointer();
	
	this->transactionStatus = TWI_InProcess;
}

void HD44780_PCF8574::Print(const char *text, uint8_t slot, uint8_t line)
{
	if(CursorOverside(slot, line)) return;
	SetCursorPosition(slot, line);
	
	while(bufferCounter == LCD_BUF_SIZE);	
	
	uint8_t length = strlen(text);
	NormaliseLength(length, slot);
	PrepareBuffer(length * sizeof(uint32_t));	

	for(uint8_t i = 0; i < this->dataSize[this->writePointer]; i += sizeof(uint32_t))
	{		
		uint32_t _data = FormByte(*text++, LCD_Data);		
				
		memcpy(buffer[this->writePointer] + i, (uint8_t*)&_data, sizeof(uint32_t));
	}
	
	this->delay[this->writePointer] = 50;
	
	UpdateWritePointer();
	
	this->transactionStatus = TWI_InProcess;
}

void HD44780_PCF8574::PrintAlignment(const char* text, uint8_t line, HD44780_Alignment alignment)
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
			slot = symbols - strlen(text);
		}break;
		
		case CENTER_SCREEN:
		{
			slot = (symbols - strlen(text)) / 2;
		}break;
	}
	
	Print(text, slot, line);
}

void HD44780_PCF8574::InsertSymbol(uint8_t address, const char *symbol)
{
	AddCommand(0b01000000 | (address << 3), 50);
	
	PrepareBuffer(7 * sizeof(uint32_t));

	for(uint8_t i = 0; i < this->dataSize[this->writePointer]; i += sizeof(uint32_t))
	{
		uint32_t _data = FormByte(*symbol++, LCD_Data);
		
		memcpy(buffer[this->writePointer] + i, (uint8_t*)&_data, sizeof(uint32_t));
	}
	
	this->delay[this->writePointer] = 50;
	
	UpdateWritePointer();
	
	this->transactionStatus = TWI_InProcess;
}

HD44780_PCF8574::~HD44780_PCF8574()
{
	for(uint8_t i = 0; i < LCD_BUF_SIZE; i++)
	{
		free((uint8_t*)buffer[i]);
	}
}

void* HD44780_PCF8574::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void HD44780_PCF8574::operator delete(void* ptr)
{
	free(ptr);
}

uint32_t HD44780_PCF8574::ClockFrequency()
{
	return PCF8574_FCLK;
}

TWI_TransactionStatus HD44780_PCF8574::GetTransactionStatus()
{
	TWI_TransactionStatus temp = this->transactionStatus;
		
	if(this->transactionStatus != TWI_InProcess)
		this->transactionStatus = TWI_Ready;
		
	return temp;
}

void HD44780_PCF8574::NextOperation(TWI_Status status)
{
	this->status = status;
		
	switch(this->status)
	{
		case TWI_NoActions: return;
		
		case TWI_ArbitrationLost:
		{
			this->twi->Stop();
			
			ClearDataCounter();
			
			this->twi->Start();
		}return;
		
		case TWI_BusError:
		case TWI_MasterAddressWriteNACK:
		case TWI_MasterDataWriteNACK:
		case TWI_MasterAddressReadNACK:
		{
			ClearDataCounter();
			
			this->twi->DisableInterrupt();
		}return;
		
		case TWI_Start:
		{
			this->twi->WriteToTWDR(this->address);
		}break;
		
		case TWI_MasterAddressWriteACK:
		case TWI_MasterDataWriteACK:
		{
			if(this->bufferCounter > 0)
			{
				if(this->dataCounter < this->dataSize[this->readPointer])
				{
					uint16_t _delay = this->delay[this->readPointer];
					while(_delay > 0)
					{
						_delay_us(50);
						_delay -= 50;
					}
					
					this->twi->WriteToTWDR(*(this->buffer[this->readPointer] + this->dataCounter));
					this->dataCounter++;
				}
				
				else
				{
					UpdateReadPointer();					
				}
			}
				
			else
			{				
				this->twi->Stop();
				
				this->transactionStatus = TWI_Ok;
			}		
		}break;
		
		default: break;
	}
}

void HD44780_PCF8574::ClearDataCounter()
{
	this->dataCounter = 0;
}

void HD44780_PCF8574::PrepareBuffer(uint8_t dataSize)
{
	this->dataSize[this->writePointer] = dataSize;
	
	this->buffer[this->writePointer] = nullptr;

	do
	{
		this->buffer[this->writePointer] = (uint8_t *)malloc(this->dataSize[this->writePointer]);
	} while (this->buffer[this->writePointer] == nullptr);	
}

void HD44780_PCF8574::SetCursorPosition(uint8_t slot, uint8_t line)
{
	if(line > 1)
	{
		line %= 2;
		slot += symbols;
	}

	uint8_t cursorAddress = 0x40 * line;
	cursorAddress += slot;
	cursorAddress |= 0x80;
	
	AddCommand(cursorAddress, 50);
}

uint32_t HD44780_PCF8574::FormByte(uint8_t data, HD44780_DataType dataType)
{
	uint32_t _data = 0;
	uint8_t *dataPtr = (uint8_t *)&_data;
	
	for (uint8_t i = 1; i <= 4; i++)
	{		
		*dataPtr |= ((data&(1<<4))>>4)<<LCD_D4 |
					((data&(1<<5))>>5)<<LCD_D5 |
					((data&(1<<6))>>6)<<LCD_D6 |
					((data&(1<<7))>>7)<<LCD_D7 |
					(1<<LCD_LED);
		
		*dataPtr &= ~(1<<LCD_RW);
		
		if(dataType == LCD_Command) *dataPtr &= ~(1<<LCD_RS);
		else if(dataType == LCD_Data) *dataPtr |= (1<<LCD_RS);
		
		if((i % 2) == 0)
		{
			*dataPtr &= ~(1<<LCD_E);
			
			data <<= 4;
		}
		
		else
		{
			*dataPtr |= (1<<LCD_E);
		}
		
		dataPtr++;
	}
	
	return _data;
}

void HD44780_PCF8574::UpdateWritePointer()
{
	++this->writePointer %= LCD_BUF_SIZE;
	
	this->bufferCounter++;
}

void HD44780_PCF8574::UpdateReadPointer()
{
	free((uint8_t*)this->buffer[this->readPointer]);
	
	++this->readPointer %= LCD_BUF_SIZE;
	
	ClearDataCounter();
	
	this->bufferCounter--;
}

void HD44780_PCF8574::AddCommand(uint8_t command, uint16_t delay)
{	
	while(bufferCounter == LCD_BUF_SIZE);
		
	PrepareBuffer(sizeof(uint32_t));
		
	uint32_t _data = FormByte(command, LCD_Command);
		
	memcpy(buffer[this->writePointer], (uint8_t*)&_data, sizeof(uint32_t));
		
	this->delay[this->writePointer] = delay;
		
	UpdateWritePointer();
		
	this->transactionStatus = TWI_InProcess;
}

bool HD44780_PCF8574::CursorOverside(uint8_t slot, uint8_t line)
{
	return (slot >= this->symbols) || (line >= this->lines);
}

void HD44780_PCF8574::NormaliseLength(uint8_t &length, uint8_t slot)
{
	uint8_t *_length = (uint8_t *)&length;
	*_length = slot + *_length <= this->symbols ? *_length : this->symbols - slot;
}
