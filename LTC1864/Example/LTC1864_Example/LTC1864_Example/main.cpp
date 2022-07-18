/*
 * LTC1864_Example.cpp
 *
 * Created: 13.07.2022 14:28:18
 * Author : Алексей
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Port.h"
#include "HD44780.h"
#include "LTC1864.h"
#include "StringHandler.h"

bool allowedMeasuring = false;
uint16_t displayedValue = 0;

ISR(TIMER0_OVF_vect)
{
	if(!allowedMeasuring) allowedMeasuring = true;
}


int main(void)
{
	Port *rs = new Port(&PORTD, &DDRD, &PIND, 0);
	Port *rw = new Port(&PORTD, &DDRD, &PIND, 1);
	Port *e = new Port(&PORTD, &DDRD, &PIND, 2);
	Port *d4 = new Port(&PORTD, &DDRD, &PIND, 3);
	Port *d5 = new Port(&PORTD, &DDRD, &PIND, 4);
	Port *d6 = new Port(&PORTD, &DDRD, &PIND, 5);
	Port *d7 = new Port(&PORTD, &DDRD, &PIND, 6);
	
	HD44780 *lcd = new HD44780(rs, rw, e, d4, d5, d6, d7, 16);
	
	Port *sck = new Port(&PORTB, &DDRB, &PINB, 5);
	Port *mosi = new Port(&PORTB, &DDRB, &PINB, 3);
	Port *miso = new Port(&PORTB, &DDRB, &PINB, 4);
	Port *ss = new Port(&PORTB, &DDRB, &PINB, 2);
	
	SPI_Master *spi = new SPI_Master(sck, mosi, miso, ss);
	
	Port *conv = new Port(&PORTB, &DDRB, &PINB, 0);
	
	LTC1864 *adc = new LTC1864(conv);
	
	//T0
	TCCR0|=(1<<CS02)|(1<<CS00);	//prescaler 1024
	TIMSK|=(1<<TOIE0);			//enable interrupt OVF
	
	sei();
	
	/* Replace with your application code */
	while (1)
	{
		if(allowedMeasuring)
		{
			allowedMeasuring = false;
			
			uint16_t measuredValue = adc->GetResult(spi);
			
			if(measuredValue != displayedValue)
			{
				displayedValue = measuredValue;
				
				lcd->Clear();
				
				const char* txt = StringHandler::FloatToString(measuredValue, 0);
				
				lcd->PrintTextWithAlignment(txt, 1, RIGHT);
				
				free((char*)txt);
				txt = nullptr;
			}
		}
	}
}

