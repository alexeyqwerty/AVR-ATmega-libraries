/*
 * MCP3202_Example.cpp
 *
 * Created: 13.07.2022 15:24:22
 * Author : Алексей
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Port.h"
#include "HD44780.h"
#include "MCP3202.h"
#include "StringHandler.h"

bool allowedPrint = false;
uint16_t displayedValue1 = 0;
uint16_t displayedValue2 = 0;

Port *rs = new Port(&PORTD, &DDRD, &PIND, 0);
Port *rw = new Port(&PORTD, &DDRD, &PIND, 1);
Port *e = new Port(&PORTD, &DDRD, &PIND, 2);
Port *d4 = new Port(&PORTD, &DDRD, &PIND, 3);
Port *d5 = new Port(&PORTD, &DDRD, &PIND, 4);
Port *d6 = new Port(&PORTD, &DDRD, &PIND, 5);
Port *d7 = new Port(&PORTD, &DDRD, &PIND, 6);
	
HD44780 *lcd = new HD44780(rs, rw, e, d4, d5, d6, d7, WH1602);
	
Port *sck = new Port(&PORTB, &DDRB, &PINB, 5);
Port *mosi = new Port(&PORTB, &DDRB, &PINB, 3);
Port *miso = new Port(&PORTB, &DDRB, &PINB, 4);
Port *ss = new Port(&PORTB, &DDRB, &PINB, 2);
	
SPI_Master *spi = new SPI_Master(sck, mosi, miso, ss);
	
Port *cs = new Port(&PORTB, &DDRB, &PINB, 0);
	
MCP3202 *adc1 = new MCP3202(cs, 0);
MCP3202 *adc2 = new MCP3202(cs, 1);

ISR(TIMER0_OVF_vect)
{
	spi->CheckDevices();
	
	if(!allowedPrint) allowedPrint = true;
}

ISR(SPI_STC_vect)
{
	spi->NextOperation();
}

void PrintResult(uint8_t line, uint16_t result, uint16_t &displayedValue)
{	
	if(displayedValue == result) return;
	
	displayedValue = result;
	
	lcd->ClearArea(5, 0, line);
	
	const char* txt = StringHandler::FloatToString(result, 0);
	
	lcd->PrintAlignment(txt, line, LEFT_SIDE);
	
	free((char*)txt);
	txt = nullptr;	
}

int main(void)
{	
	//T0
	TCCR0|=(1<<CS02)|(1<<CS00);	//prescaler 1024
	TIMSK|=(1<<TOIE0);			//enable interrupt OVF
	
	spi->AddDevice(adc1);
	spi->AddDevice(adc2);
	
	sei();
	
	/* Replace with your application code */
	while (1)
	{
		if(allowedPrint)
		{
			allowedPrint = false;
			
			PrintResult(0, adc1->GetResult(), displayedValue1);
			
			PrintResult(1, adc2->GetResult(), displayedValue2);
		}
	}
}

