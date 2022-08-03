#include <avr/io.h>
#include <avr/interrupt.h>

#include "Port.h"
#include "SPI_Master.h"
#include "MCP4921.h"

Port *sck = new Port(&PORTB, &DDRB, &PINB, 5);
Port *mosi = new Port(&PORTB, &DDRB, &PINB, 3);
Port *miso = new Port(&PORTB, &DDRB, &PINB, 4);
Port *ss = new Port(&PORTB, &DDRB, &PINB, 2);
	
SPI_Master *spi = new SPI_Master(sck, mosi, miso, ss);

Port *ldac = new Port(&PORTB, &DDRB, &PINB, 0);
Port *cs_1 = new Port(&PORTB, &DDRB, &PINB, 1);
Port *cs_2 = new Port(&PORTB, &DDRB, &PINB, 2);

MCP4921 *dac_1 = new MCP4921(cs_1, ldac, 5);
MCP4921 *dac_2 = new MCP4921(cs_2, ldac, 5);

uint16_t data_1 = 0;
uint16_t data_2 = 4096;

ISR(TIMER1_OVF_vect)
{
	data_1++;
	data_2--;
	
	dac_1->SetOutput(data_1);
	dac_2->SetOutput(data_2);
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
		
	spi->AddDevice(dac_1);	
	spi->AddDevice(dac_2);
	
	sei();	
		
	while (1)
	{
		
	}
}