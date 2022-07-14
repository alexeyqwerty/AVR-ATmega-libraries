/*
 * CD4051_Example.cpp
 *
 * Created: 14.07.2022 10:39:50
 * Author : Алексей
 */ 

#include <avr/io.h>
#include "Port.h"
#include "CD4051.h"

#define  F_CPU 8000000
#include <util/delay.h>

void Present(CD4051 *mux)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		mux->SelectChannel(i);
		mux->EnableOutputs();
		_delay_ms(1000);
		mux->DisableOutputs();
	}
}

int main(void)
{
	Port *a = new Port(&PORTB, &DDRB, &PINB, 0);
	Port *b = new Port(&PORTB, &DDRB, &PINB, 1);
	Port *c = new Port(&PORTB, &DDRB, &PINB, 2);
	
	Port *inh1 = new Port(&PORTB, &DDRB, &PINB, 3);
	Port *inh2 = new Port(&PORTB, &DDRB, &PINB, 4);
	
	CD4051 *mux1 = new CD4051(a, b, c, inh1);
	CD4051 *mux2 = new CD4051(a, b, c, inh2);
	
    /* Replace with your application code */
    while (1) 
    {
		Present(mux1);
		Present(mux2);
    }
}

