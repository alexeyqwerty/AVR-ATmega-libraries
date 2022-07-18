/*
 * MatrixKeyboard_Example.cpp
 *
 * Created: 13.07.2022 13:13:34
 * Author : Алексей
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Port.h"
#include "HD44780.h"
#include "MatrixKeyboard.h"

bool allowCheckKeyboard = false;

ISR(TIMER0_OVF_vect)
{
	if(allowCheckKeyboard == false) allowCheckKeyboard = true;
}


int main(void)
{
	//T0
	TCCR0|=(1<<CS02);//|(1<<CS00);	//prescaler 1024
	TIMSK|=(1<<TOIE0);			//enable interrupt OVF
	
	Port *rs = new Port(&PORTB, &DDRB, &PINB, 0);
	Port *rw = new Port(&PORTB, &DDRB, &PINB, 1);
	Port *e = new Port(&PORTB, &DDRB, &PINB, 2);
	Port *d4 = new Port(&PORTB, &DDRB, &PINB, 3);
	Port *d5 = new Port(&PORTB, &DDRB, &PINB, 4);
	Port *d6 = new Port(&PORTB, &DDRB, &PINB, 5);
	Port *d7 = new Port(&PORTB, &DDRB, &PINB, 6);
	
	HD44780 *lcd = new HD44780(rs, rw, e, d4, d5, d6, d7, 16);
	
	Port *r0 = new Port(&PORTD, &DDRD, &PIND, 0);
	Port *r1 = new Port(&PORTD, &DDRD, &PIND, 1);
	Port *r2 = new Port(&PORTD, &DDRD, &PIND, 2);
	Port *r3 = new Port(&PORTD, &DDRD, &PIND, 3);
	
	Port *c0 = new Port(&PORTD, &DDRD, &PIND, 4);
	Port *c1 = new Port(&PORTD, &DDRD, &PIND, 5);
	Port *c2 = new Port(&PORTD, &DDRD, &PIND, 6);
	
	MatrixKeyboard *keyb = new MatrixKeyboard(r0, r1, r2, r3, c0, c1, c2);
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		if(allowCheckKeyboard == true)
		{
			allowCheckKeyboard = false;
			
			uint8_t bl = keyb->GetPressedButton();
			
			if(bl < ButtonsNum)
			{
				lcd->Clear();
				
				switch((ButtonsLayout)bl)
				{
					case D0: lcd->PrintText("0", 0, 0); break;
					case D1: lcd->PrintText("1", 0, 0); break;
					case D2: lcd->PrintText("2", 0, 0); break;
					case D3: lcd->PrintText("3", 0, 0); break;
					case D4: lcd->PrintText("4", 0, 0); break;
					case D5: lcd->PrintText("5", 0, 0); break;
					case D6: lcd->PrintText("6", 0, 0); break;
					case D7: lcd->PrintText("7", 0, 0); break;
					case D8: lcd->PrintText("8", 0, 0); break;
					case D9: lcd->PrintText("9", 0, 0); break;
					case STAR: lcd->PrintText("*", 0, 0); break;
					case HASH: lcd->PrintText("#", 0, 0); break;
				}
			}			
		}
    }
}

