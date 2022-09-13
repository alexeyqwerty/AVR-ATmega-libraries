#include "UART.h"

const char start_mark = '*';
const char end_mark = '$';

UART::UART(uint32_t baudrate, uint32_t processorFrequency, uint8_t bufferSize, uint8_t messageMaxSize)
{			
	this->messageTransmissionStarted = false;
	this->TX_CharsCounter = 0;

	this->TX_Buffer = new Buffer(bufferSize, messageMaxSize);
		
 	this->messageReceptionStarted = false;	
 	this->RX_CharsCounter = 0;

 	this->RX_Buffer = new Buffer(bufferSize, messageMaxSize);
	
	uint16_t ubrr_temp = (processorFrequency / (baudrate * 16)) - 1;
	
	_UCSRC |= (1<<_UCSZ1)|(1<<_UCSZ0);
	
	#if defined(_URSEL)
		_UCSRC |= (1<<_URSEL);
	#endif
	
	_UBRRH = (ubrr_temp>>8);
	_UBRRL = ubrr_temp;
}

void UART::Enable()
{
	_UCSRB |= (1<<_RXEN)|(1<<_TXEN)|(1<<_RXCIE)|(1<<_TXCIE);
}

void UART::Disable()
{
	_UCSRB &= ~(1<<_RXEN)&~(1<<_TXEN)&~(1<<_RXCIE)&~(1<<_TXCIE);
}

void UART::UploadingDataForTransfer(const char* data)
{	
	this->TX_Buffer->Write(*data, strlen(data));
	this->TX_Buffer->Save();
}

void UART::TransmissionStop()
{
	this->messageTransmissionStarted = false;
}

void UART::Transmit()
{	
	if(this->messageTransmissionStarted) return;
		
	if(this->TX_Buffer->BlockSize() > 0)
	{
		if(TX_CharsCounter < this->TX_Buffer->BlockSize())
		{
			_UDR = this->TX_Buffer->Read()[TX_CharsCounter++];
			messageTransmissionStarted = true;
		}
		
		else
		{
			this->TX_Buffer->Next();
			this->TX_CharsCounter = 0;
		}
	}
}

void UART::Receive()
{
	uint8_t data = _UDR;

	if(data == start_mark)
	{
		this->messageReceptionStarted = true;
		
		return;
	}
	
	else if (data == end_mark)
	{	
		this->RX_Buffer->Save();
		
		this->messageReceptionStarted = false;
		
		this->RX_CharsCounter = 0;		
		
		return;
	}

	if(this->messageReceptionStarted == true)
	{
		this->RX_Buffer->Write(data, sizeof(data));
	}
}

bool UART::HaveUnreadMessages()
{
	return this->RX_Buffer->BlockSize() > 0;
}

const char* UART::GetRX_Message()
{
	return (const char*)this->RX_Buffer->Read();
}

void UART::ClearRX_Message()
{
	this->RX_Buffer->Next();
}

UART::~UART()
{
	delete this->TX_Buffer;
	this->TX_Buffer = nullptr;
	
	delete this->RX_Buffer;
	this->RX_Buffer = nullptr;
}

void* UART::operator new(size_t size)
{
	void* ptr;
	
	do
		ptr = malloc(size);
	while(ptr == nullptr);
	
	return ptr;
}

void UART::operator delete(void* ptr)
{
	free(ptr);
	ptr = nullptr;
}
