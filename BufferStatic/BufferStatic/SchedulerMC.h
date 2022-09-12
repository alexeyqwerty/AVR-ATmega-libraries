#ifndef SCHEDULERMC_H_
#define SCHEDULERMC_H_

#include <avr/io.h>

#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega8A__)
	#define _TCCR0 TCCR0
	#define _OCR0 OCR0
	#define _OCIE0 OCIE0
	#define _TIMSK TIMSK
	#define SCHEDULER_INTERRUPT TIMER0_COMP_vect

#elif defined(__AVR_ATmega16__) || defined (__AVR_ATmega16A__)
	#define _TCCR0 TCCR0
	#define _OCR0 OCR0
	#define _OCIE0 OCIE0
	#define _TIMSK TIMSK
	#define SCHEDULER_INTERRUPT TIMER0_COMP_vect

#elif defined(__AVR_ATmega32__) || defined (__AVR_ATmega32A__)
	#define _TCCR0 TCCR0
	#define _OCR0 OCR0
	#define _OCIE0 OCIE0
	#define _TIMSK TIMSK
	#define SCHEDULER_INTERRUPT TIMER0_COMP_vect

#elif (defined __AVR_ATmega328__) || defined(__AVR_ATmega328P__)
	#define _TCCR0 TCCR0B
	#define _OCR0 OCR0A
	#define _OCIE0 OCIE0A
	#define _TIMSK TIMSK0
	#define SCHEDULER_INTERRUPT TIMER0_COMPA_vect
	
#elif defined (__AVR_ATmega644__) || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__)
	#define _TCCR0 TCCR0B
	#define _OCR0 OCR0A
	#define _OCIE0 OCIE0A
	#define _TIMSK TIMSK0
	#define SCHEDULER_INTERRUPT TIMER0_COMPA_vect

#elif defined (__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
	#define _TCCR0 TCCR0B
	#define _OCR0 OCR0A
	#define _OCIE0 OCIE0A
	#define _TIMSK TIMSK0
	#define SCHEDULER_INTERRUPT TIMER0_COMPA_vect

#endif


#endif /* SCHEDULERMC_H_ */