/************************************************************************************
Title:						USART library with using RX/TX interrupts

Supported microcontrollers: ATmega16, ATmega32
							ATmega328P
							ATmega644P, ATmega1284P(USART0)
		 
Notes:						- Working mode: 8 bit, no parity, 1 stop, asynchronous	
							- 2 buffers of 10 messages for receiving and transmitting
							- 30 characters per message	
*************************************************************************************/

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
	#define USART_ATMEGA
	#define USART_RX_INTERRUPT USART_RXC_vect
	#define USART_TX_INTERRUPT USART_TXC_vect
	#define _UDR UDR

#elif defined(__AVR_ATmega328P__)
	#define USART0_ATMEGA
	#define USART_RX_INTERRUPT USART_RX_vect
	#define USART_TX_INTERRUPT USART_TX_vect
	#define _UDR UDR0

#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
	#define USART0_ATMEGA
	#define USART_RX_INTERRUPT USART0_RX_vect
	#define USART_TX_INTERRUPT USART0_TX_vect
	#define _UDR UDR0
#endif

#define maxMessages 20
#define maxWords 30

struct Message
{
	char messageText[maxWords];
	uint8_t messageLength;
};

enum UART_Status
{
	UART_OFF,
	UART_ON
};
	
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
	UART(uint32_t baudrate, uint32_t processorFrequency);
	
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
	Function:		GetStatus()
	Purpose:		Reporting USART active status
	Input:			No
	Returns:		USART active status
	************************************************************************/
	UART_Status GetStatus();	
	
	/***********************************************************************
	Function:		UploadigDataForTransfer()
	Purpose:		Saving messages to send in TX messages array
	Input:			- Data to transmitting
	Returns:		Transmission report
	************************************************************************/	
	void UploadigDataForTransfer(const char* data);	
	
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
	void ReceiveData();	
	
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
	
	private:
	
	UART_Status status;
	
	struct Message TX_Messages[maxMessages];
	char* TX_Message;
	uint8_t TX_MessagesCount;
	uint8_t TX_CharsCounter;
	bool messageTransmissionStarted;
	uint8_t TX_ReadPointer;
	uint8_t TX_WritePointer;

	struct Message RX_Messages[maxMessages];
	char* RX_Message;
	uint8_t RX_MessagesCount;
	bool messageReceptionStarted;
	uint8_t RX_ReadPointer;
	uint8_t RX_WritePointer;
};

#endif

/************************************************************************
							HOW IT USE        							

					-=Creating the UART object=-

UART uart(_some_baudreit_, _some_f_cpu_);                                   

*************************************************************************
					-=Using RXC and TXC interrupts=-

ISR(USART_RX_INTERRUPT)
{
	uart.ReceiveData();	<- Here's writing into the RX_Messages array
}

ISR(USART_TX_INTERRUPT)
{
	uart.TransmissionStop(); <- Here's changing transmission started flag
}

************************************************************************

uart.Enable(); <- Don't forget enable UART before using

sei(); <- Also global interrupts must be active

************************************************************************
					-=Insert data in TX_Messages array=-
	
UART_TX_Report UploadigDataForTransfer(const char* data);

************************************************************************
						-=In some cycle=-

if(uart.HaveUnreadMessages()) <- Here check having unread messages
{
	const char* message = uart.GetRX_MessageType())	<- Here's getting the message type
	
	uart.ClearRX_Message(); <- Here's deleting read message. Tt's NECESSARY!!!
}
		
if(!uart.TransmissionStarted()) <- The transfer should not be started
{
	uart.Transmit(); <- Here's transmitting data from TX_Messages array
}

************************************************************************/