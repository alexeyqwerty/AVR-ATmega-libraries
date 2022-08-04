#include <avr/io.h>
#include <avr/interrupt.h>

#include "TWI.h"
#include "HD44780_PCF8574.h"

#define Rectangle (const char[]){0b00011111, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011111}
#define RectAddr 1
#define RectangleSymbol (const char[]){RectAddr, '\0'}


TWI *twi = new TWI(F_CPU);

HD44780_PCF8574 *lcd = new HD44780_PCF8574(twi, 0, 0, 1, WH2004);


ISR(TWI_vect)
{
	twi->HandleDataChange();
}

ISR(TIMER0_OVF_vect)
{
	twi->CheckDevices();
}

int main(void)
{
	//T0
	TCCR0|=(1<<CS02)|(1<<CS00);	//prescaler 1024
	TIMSK|=(1<<TOIE0);			//enable interrupt OVF

	twi->AddDevice(lcd);
		
	sei();
		
	lcd->InsertSymbol(RectAddr, Rectangle);
		
	lcd->Print("hello", 5, 0);
	lcd->PrintAlignment("alignment", 2, CENTER_SCREEN);
	lcd->Print("overflow", 17, 3);
		
	lcd->Print(RectangleSymbol, 0, 3);


    while (1) 
    {
    }
}

