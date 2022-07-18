#include "UART.h"

const char start_mark = '*';
const char end_mark = '$';

UART::UART(uint32_t baudrate, uint32_t processorFrequency)
{	
	status = UART_OFF;
	
	TX_MessagesCount = 0;
	TX_ReadPointer = 0;
	TX_CharsCounter = 0;
	TX_WritePointer = 0;
	messageTransmissionStarted = false;	
	
	RX_MessagesCount = 0;
	RX_ReadPointer = 0;
	RX_WritePointer = 0;	
	messageReceptionStarted = false;		
	
	for(uint8_t i = 0; i < maxMessages; i++)
	{
		TX_Messages[i].messageLength = 0;				
		
		RX_Messages[i].messageLength = 0;		
		
		for(uint8_t j = 0; j < maxWords; j++)
		{
			TX_Messages[i].messageText[j] = '\0';
			RX_Messages[i].messageText[j] = '\0';
		}				
	}
	
	uint16_t ubrr_temp = (processorFrequency / (baudrate * 16)) - 1;
		
	#if defined(USART_ATMEGA)
		UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);		
		UBRRH = (ubrr_temp>>8);
		UBRRL = ubrr_temp;
			
	#elif defined(USART0_ATMEGA)
		UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);	
		UBRR0H = (ubrr_temp>>8);
		UBRR0L = ubrr_temp;		
		
	#endif		
}

void UART::Enable()
{
	#if defined(USART_ATMEGA)
		UCSRB |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<TXCIE);
			
	#elif defined(USART0_ATMEGA)
		UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);	
		
	#endif	
	
	status = UART_ON;
}

void UART::Disable()
{
	#if defined(USART_ATMEGA)
		UCSRB &= ~(1<<RXEN)&~(1<<TXEN)&~(1<<RXCIE)&~(1<<TXCIE);
		
	#elif defined(USART0_ATMEGA)
		UCSR0B &= ~(1<<RXEN0)&~(1<<TXEN0)&~(1<<RXCIE0)&~(1<<TXCIE0);	
		
	#endif
	
	status = UART_OFF;
}

UART_Status UART::GetStatus()
{
	return status;
}

void UART::UploadigDataForTransfer(const char* data)
{	
	if(TX_MessagesCount < maxMessages)
	{
		uint8_t arrayLength = strlen(data);
		
		if(arrayLength > maxWords) return;				
		
		memcpy(TX_Messages[TX_WritePointer].messageText, data, arrayLength);
		
		TX_Messages[TX_WritePointer].messageLength = arrayLength;
		
		TX_WritePointer = (TX_WritePointer + 1) % maxMessages;
		TX_MessagesCount++;	
	}
}

void UART::TransmissionStop()
{
	messageTransmissionStarted = false;
}

void UART::Transmit()
{	
	if(messageTransmissionStarted) return;
		
	if(TX_MessagesCount > 0)
	{
		if(TX_CharsCounter == 0)
		{
			do 
			{
				TX_Message = (char*)malloc(TX_Messages[TX_ReadPointer].messageLength);
			} while(TX_Message == NULL);
			
			memcpy(TX_Message, TX_Messages[TX_ReadPointer].messageText, TX_Messages[TX_ReadPointer].messageLength);
		}
		
		if(TX_CharsCounter < TX_Messages[TX_ReadPointer].messageLength)
		{
			_UDR = TX_Message[TX_CharsCounter];			
			
			TX_CharsCounter++;
			
			messageTransmissionStarted = true;
		}
		
		else
		{
			free(TX_Message);
			
			TX_CharsCounter = 0;
			TX_ReadPointer = (TX_ReadPointer + 1) % maxMessages;
			TX_MessagesCount--;
		}
	}
}

void UART::ReceiveData()
{
	char data = _UDR;

	if((data == start_mark) && (RX_MessagesCount < maxMessages))
	{
		messageReceptionStarted = true;
		
		RX_Messages[RX_WritePointer].messageLength = 0;
		
		return;
	}
	
	else if (data == end_mark)
	{
		if(messageReceptionStarted == false) return;
	
		RX_WritePointer = (RX_WritePointer + 1) % maxMessages;
		RX_MessagesCount++;
		
		messageReceptionStarted = false;
		return;
	}

	if(messageReceptionStarted == true)
	{
		RX_Messages[RX_WritePointer].messageText[RX_Messages[RX_WritePointer].messageLength] = data;
		RX_Messages[RX_WritePointer].messageLength++;
		
		if(RX_Messages[RX_WritePointer].messageLength == maxWords)
		{
			RX_Messages[RX_WritePointer].messageLength = 0;
			
			messageReceptionStarted = false;
		}
	}
}

bool UART::HaveUnreadMessages()
{
	return (RX_MessagesCount > 0);	
}

const char* UART::GetRX_Message()
{
	do 
	{
		RX_Message = (char*)malloc(RX_Messages[RX_ReadPointer].messageLength);
	} while(RX_Message == NULL);
	
	memcpy(RX_Message, RX_Messages[RX_ReadPointer].messageText, RX_Messages[RX_ReadPointer].messageLength);
	
	return RX_Message;
}

void UART::ClearRX_Message()
{		
	free(RX_Message);
	
	RX_ReadPointer = (RX_ReadPointer + 1) % maxMessages;
	RX_MessagesCount--;
}

UART::~UART()
{

}

void* UART::operator new(size_t size)
{
	void* ptr;
	do 
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void UART::operator delete(void* ptr)
{
	if(ptr != nullptr) free(ptr);
}
