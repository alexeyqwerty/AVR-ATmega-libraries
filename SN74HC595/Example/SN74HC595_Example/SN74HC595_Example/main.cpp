#include <avr/io.h>
#include <avr/interrupt.h>

#include "Port.h"
#include "SPI_Master.h"
#include "SN74HC595.h"

Port *sck = new Port(&PORTB, &DDRB, &PINB, 5);
Port *mosi = new Port(&PORTB, &DDRB, &PINB, 3);
Port *miso = new Port(&PORTB, &DDRB, &PINB, 4);
Port *ss = new Port(&PORTB, &DDRB, &PINB, 2);

SPI_Master *spi = new SPI_Master(sck, mosi, miso, ss);

Port *latch_1 = new Port(&PORTB, &DDRB, &PINB, 0);
Port *latch_2 = new Port(&PORTB, &DDRB, &PINB, 1);

SN74HC595 *reg_1 = new SN74HC595(latch_1, 5);
SN74HC595 *reg_2 = new SN74HC595(latch_2, 5);

uint16_t data_1 = 0;
uint16_t data_2 = 1;

ISR(TIMER1_OVF_vect)
{
	data_1++;
	data_2 += 2;	
	reg_1->SetOutput(data_1, sizeof(data_1));
	reg_2->SetOutput(data_2, sizeof(data_2));
}

ISR(TIMER0_OVF_vect)
{
	spi->CheckDevices();
}

ISR(SPI_STC_vect)
{
	spi->NextOperation();
}

int main(void)
{
	//T0
	TCCR0|=(1<<CS02)|(1<<CS00);	//prescaler 1024
	TCCR1B|=(1<<CS11)|(1<<CS10);
	TIMSK|=(1<<TOIE0)|(1<<TOIE1);			//enable interrupt OVF
		
	spi->AddDevice(reg_1);
	spi->AddDevice(reg_2);
	
	sei();
	
	while (1)
	{
		
	}
}