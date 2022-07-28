/*
 * GccApplication1.cpp
 *
 * Created: 15.07.2022 3:45:54
 * Author : Алексей
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000
#include "Port.h"
#include "HD44780.h"
#include "StringHandler.h"
#include "AT24C256.h"
#include "TWI.h"

Port *rs = new Port(&PORTB, &DDRB, &PINB, 0);
Port *rw = new Port(&PORTB, &DDRB, &PINB, 1);
Port *e = new Port(&PORTB, &DDRB, &PINB, 2);
Port *d4 = new Port(&PORTB, &DDRB, &PINB, 3);
Port *d5 = new Port(&PORTB, &DDRB, &PINB, 4);
Port *d6 = new Port(&PORTB, &DDRB, &PINB, 5);
Port *d7 = new Port(&PORTB, &DDRB, &PINB, 6);
 	
HD44780 *lcd = new HD44780(rs, rw, e, d4, d5, d6, d7, 16);

TWI *twi = new TWI(F_CPU);
AT24C256 *eep1 = new AT24C256(twi, 1, 1, 1, 50000);
AT24C256 *eep2 = new AT24C256(twi, 0, 1, 1, 50000);
AT24C256 *eep3 = new AT24C256(twi, 0, 0, 0, 50000);

bool allowPrintE1 = true;
bool allowPrintE2 = true;
bool allowPrintE3 = true;

uint16_t e1 = 0;
float e2 = 0;
long e3 = 0;

ISR(TWI_vect)
{
	twi->HandleDataChange();
}

ISR(TIMER0_OVF_vect)
{
	twi->CheckDevices();
	
	if(allowPrintE1)
	{
		TWI_TransactionStatus ts1 = eep1->GetTransactionStatus();
		if(ts1 == TWI_Ok)
		{
			allowPrintE1 = false;
			lcd->PrintText(StringHandler::FloatToString(e1, 0), 0, 0);			
		}
		else if(ts1 == TWI_Error)
		{
			allowPrintE1 = false;
			lcd->PrintText("err1", 0, 0);	
		}
	}
	
	if(allowPrintE2)
	{
		TWI_TransactionStatus ts2 = eep2->GetTransactionStatus();
		if(ts2 == TWI_Ok)
		{
			allowPrintE2 = false;
			lcd->PrintText(StringHandler::FloatToString(e2, 5), 0, 1);
		}
		else if(ts2 == TWI_Error)
		{
			allowPrintE2 = false;
			lcd->PrintText("err2", 0, 1);
		}
	}
	
	if(allowPrintE3)
	{
		TWI_TransactionStatus ts3 = eep3->GetTransactionStatus();
		if(ts3 == TWI_Ok)
		{
			allowPrintE3 = false;
			lcd->PrintText(StringHandler::FloatToString(e3, 0), 10, 0);
		}
		else if(ts3 == TWI_Error)
		{
			allowPrintE3 = false;
			lcd->PrintText("err3", 10, 0);
		}
	}
}

int main(void)
{
	
	//T0
	TCCR0|=(1<<CS02)|(1<<CS00);	//prescaler 1024
	TIMSK|=(1<<TOIE0);			//enable interrupt OVF
		
	sei();
	
	twi->AddDevice(eep1);
	twi->AddDevice(eep2);
	twi->AddDevice(eep3);
	
  	//e1 = 101;
  	//eep1->SetDataToWrite(0, e1, sizeof(e1));
  	//
  	//e2 = -0.37912;
  	//eep2->SetDataToWrite(50, e2, sizeof(e2));
	//
  	//e3 = 338542;
  	//eep3->SetDataToWrite(8150, e3, sizeof(e3));
	
  	eep1->SetDataToRead(0, e1, sizeof(e1));
  	eep2->SetDataToRead(50, e2, sizeof(e2));
  	eep3->SetDataToRead(8150, e3, sizeof(e3));	 
	
	/* Replace with your application code */
	while (1)
	{
	}
}