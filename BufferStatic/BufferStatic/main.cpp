#include <avr/io.h>
#include <avr/interrupt.h>
#include "Scheduler.h"
#include "UART.h"
#include "StringHandler.h"

#define F_CPU 8000000
#define UART_BAUD 9600

Scheduler *sch = new Scheduler(F_CPU, 5);
UART *uart = new UART(UART_BAUD, F_CPU, 10, 10);

ISR(SCHEDULER_INTERRUPT)
{
	sch->UpdateLeftTime();
}

ISR(USART_RX_INTERRUPT)
{
	uart->Receive();
}

ISR(USART_TX_INTERRUPT)
{
	uart->TransmissionStop();
	uart->Transmit();
}

uint32_t sec = 0;

void S()
{
	sec++;
}

void T()
{
	uart->Transmit();
}

void F3()
{
	PORTC ^= (1<<2);
	
	if(uart->HaveUnreadMessages())
	{
		uart->UploadingDataForTransfer(uart->GetRX_Message());
		uart->ClearRX_Message();
	}
}

void F2()
{
	PORTC ^= (1<<1);
	uart->UploadingDataForTransfer("F2 - ");
	const char *s = StringHandler::FloatToString(sec, 0);
	uart->UploadingDataForTransfer(s);
	free((char*)s);
	s = nullptr;
	uart->UploadingDataForTransfer(" sec\r");
}

void F1()
{
	PORTC ^= (1<<0);
	uart->UploadingDataForTransfer("F1 - ");
	const char *s = StringHandler::FloatToString(sec, 0);
	uart->UploadingDataForTransfer(s);
	free((char*)s);
	s = nullptr;
	uart->UploadingDataForTransfer(" sec\r");
}



int main(void)
{
	uart->Enable();
	
	sch->AddTask(S, 1000);
	sch->AddTask(F1, 1000);
	sch->AddTask(T, 10);
	sch->AddTask(F2, 2000);
	sch->AddTask(F3, 500);	
		
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		sch->DispatchTasks();
    }
}

