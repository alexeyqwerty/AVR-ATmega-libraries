#include <avr/io.h>
#include <avr/interrupt.h>

#include "TWI.h"
#include "PCF8574.h"

#define F_CPU 8000000

TWI *twi = new TWI(F_CPU);

PCF8574 *expanderRead = new PCF8574(twi, 1, 1, 1, 50000);
PCF8574 *expanderWrite = new PCF8574(twi, 1, 1, 0, 50000);

uint8_t expR = 0;
uint8_t expW = 0;

ISR(TWI_vect)
{
	twi->HandleDataChange();
}

ISR(TIMER0_OVF_vect)
{
	twi->CheckDevices();
	
	if(expW != expR)
	{
		expW = expR;
		expanderWrite->SetDataToWrite(expW, sizeof(expW));
	}
	
	expanderRead->StartRead(expR);
}

int main(void)
{
	//T0
	TCCR0|=(1<<CS02)|(1<<CS00);	//prescaler 1024
	TIMSK|=(1<<TOIE0);			//enable interrupt OVF
	
	twi->AddDevice(expanderRead);
	twi->AddDevice(expanderWrite);	
	
	sei();
	
    while (1) 
    {
    }
}

