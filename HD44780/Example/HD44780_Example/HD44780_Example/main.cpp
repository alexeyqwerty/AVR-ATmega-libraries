/*
 * HD44780_Example.cpp
 *
 * Created: 13.07.2022 13:06:16
 * Author : Алексей
 */ 

#include <avr/io.h>
#include "Port.h"
#include "HD44780.h"


int main(void)
{
	Port *rs = new Port(&PORTB, &DDRB, &PINB, 0);
	Port *rw = new Port(&PORTB, &DDRB, &PINB, 1);
	Port *e = new Port(&PORTB, &DDRB, &PINB, 2);
	Port *d4 = new Port(&PORTB, &DDRB, &PINB, 3);
	Port *d5 = new Port(&PORTB, &DDRB, &PINB, 4);
	Port *d6 = new Port(&PORTB, &DDRB, &PINB, 5);
	Port *d7 = new Port(&PORTB, &DDRB, &PINB, 6);
	
	HD44780 *lcd = new HD44780(rs, rw, e, d4, d5, d6, d7, WH1602);
	
	lcd->Print("Hello my friend", 0, 1);
	lcd->PrintAlignment("CENTER", 0, CENTER_SCREEN);
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

