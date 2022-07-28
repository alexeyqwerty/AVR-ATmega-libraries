#include <avr/io.h>

#include "Port.h"
#include "SPI_Master.h"
#include "MCP4921.h"

Port *sck = new Port(&PORTB, &DDRB, &PINB, 5);
Port *mosi = new Port(&PORTB, &DDRB, &PINB, 3);
Port *miso = new Port(&PORTB, &DDRB, &PINB, 4);
Port *ss = new Port(&PORTB, &DDRB, &PINB, 2);

SPI_Master *spi = new SPI_Master(sck, mosi, miso, ss);

Port *cs = new Port(&PORTB, &DDRB, &PINB, 0);
Port *ldac = new Port(&PORTB, &DDRB, &PINB, 1);

MCP4921 *dac = new MCP4921(cs, ldac);

int main(void)
{
	dac->SetOutput(1022, spi);
	
	while (1)
	{
		
	}
}