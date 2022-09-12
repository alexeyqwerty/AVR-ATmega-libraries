/************************************************************************************
Title:						USART library with using RX/TX interrupts

Supported microcontrollers: ATmega16, ATmega32
							ATmega328P
							ATmega644P, ATmega1284P(USART0)
		 
Notes:						- Working mode: 8 bit, no parity, 1 stop, asynchronous	
							- 2 buffers for receiving and transmitting
*************************************************************************************/

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#include "Buffer.h"

#include "UART_MC.h"

class UART
{
	public:		
	
	/***********************************************************************
	Function:		UART()
	Purpose:		Initializing USART
	Input:			- Baudrate value
					- Processor tact frequency value
	Returns:		No
	************************************************************************/
	UART(uint32_t baudrate, uint32_t processorFrequency, uint8_t bufferSize, uint8_t messageMaxSize);
	
	/***********************************************************************
	Functions:		Enable()
					Disable()
	Purpose:		Enable and disable receiver and transmitter modules 
					and interrupts
	Input:			No
	Returns:		No
	************************************************************************/
	void Enable();	
	void Disable();	
	
	/***********************************************************************
	Function:		UploadigDataForTransfer()
	Purpose:		Saving messages to send in TX messages array
	Input:			- Data to transmitting
	Returns:		Transmission report
	************************************************************************/	
	void UploadingDataForTransfer(const char* data);	
	
	/***********************************************************************
	Function:		TransmissionStop()
	Purpose:		Changing transmission started flag to false, using in 
					TX complete interrupt
	Input:			No
	Returns:		NO
	************************************************************************/	
	void TransmissionStop();
	
	/***********************************************************************
	Function:		Transmit()
	Purpose:		Transmitting first message from TX messages array and
					deleting his after transmission. The second message for
					transmitting becomes first
	Input:			No
	Returns:		No
	************************************************************************/
	void Transmit();
	
	/***********************************************************************
	Function:		ReceiveData()
	Purpose:		Receiving data and saving in RX_Messages array
	Input:			No 
	Returns:		No
	************************************************************************/
	void Receive();	
	
	/***********************************************************************
	Function:		HaveUnreadMessages()
	Purpose:		Reporting about presence of unread messages in 
					RX_Messages array
	Input:			No
	Returns:		Boolean value. True - if have unread messages
	************************************************************************/
	bool HaveUnreadMessages();
	
	/***********************************************************************
	Function:		GetRX_Message()
	Purpose:		Returns first incoming message from RX_Messages array
	Input:			No
	Returns:		Message text
	************************************************************************/
	const char* GetRX_Message();
	
	/***********************************************************************
	Function:		ClearRX_Message()
	Purpose:		Delete the first incoming message from RX messages array.
					The second message becomes first.
	Input:			No
	Returns:		No
	************************************************************************/
	void ClearRX_Message();
	
	~UART();
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	private:
	
	bool messageTransmissionStarted;
	uint8_t TX_CharsCounter;	
	
	Buffer *TX_Buffer;	
	
	bool messageReceptionStarted;
	uint8_t RX_MessagesCount;
	uint8_t RX_CharsCounter;
	uint8_t *RX_Message;
	
	Buffer *RX_Buffer;
};

#endif

/************************************************************************
							HOW IT USE        							

					-=Creating the UART object=-

UART *uart = new UART(someBaudrate, someF_CPU, someBufferSize);                                   

*************************************************************************
					-=Using RXC and TXC interrupts=-

ISR(USART_RX_INTERRUPT)
{
	uart->Receive();	<- Here's writing into the RX_Buffer
}

ISR(USART_TX_INTERRUPT)
{
	uart->TransmissionStop(); <- Here's changing transmission started flag
	uart->Transmit();
}

************************************************************************

uart->Enable(); <- Don't forget enable UART before using

sei(); <- Also global interrupts must be active

************************************************************************
					-=Insert data in TX_Buffer=-
	
uart->UploadigDataForTransfer(someString);

						-=Transmit messages=-

From time to time:					

uart->Transmit(); <- Here's transmitting data from TX_Messages array

************************************************************************
						-=Receive messages=-

From time to time:

if(uart->HaveUnreadMessages()) <- Here check having unread messages
{
	const char* message = uart->GetRX_Message();	<- Here's getting the message
	
	... do something ...
	
	uart->ClearRX_Message(); <- Here's deleting read message. Tt's NECESSARY!!!
}

************************************************************************/