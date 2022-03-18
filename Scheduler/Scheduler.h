/************************************************************************************
Title:						Task scheduler

Support:					- ATmega16, Atmega32
							- ATmega328
							- ATmega644, ATmega1284							
		 
Notes:						- Minimal time unit - 1ms
							- Cannot be used for time counting							
*************************************************************************************/

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <avr/io.h>

#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
	#define TCCR TCCR0	
	#define _OCR0 OCR0
	#define _OCIE0 OCIE0
	#define _TIMSK TIMSK
	#define SCHEDULER_INTERRUPT TIMER0_COMP_vect	

#elif defined(__AVR_ATmega328P__)
	#define TCCR TCCR0B	
	#define _OCR0 OCR0A
	#define _OCIE0 OCIE0A
	#define _TIMSK TIMSK0
	#define SCHEDULER_INTERRUPT TIMER0_COMPA_vect

#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
	#define TCCR TCCR0B
	#define _OCR0 OCR0A
	#define _OCIE0 OCIE0A
	#define _TIMSK TIMSK0
	#define SCHEDULER_INTERRUPT TIMER0_COMPA_vect
#endif

/***********************************************************************
Description:		Maximum number of tasks in the queue
************************************************************************/
#define MAX_TASKS 6

/***********************************************************************
Description:		Task:
					- function - function to perform
					- executionPeriod - the interval between execution
					- timeLeft - time to execution count
************************************************************************/
struct Task
{
	void (*function)(void);
	uint16_t executionPeriod;
	uint16_t timeLeft;
};

class Scheduler
{
	public:
	/***********************************************************************
	Function:		Scheduler()
	Purpose:		Initializing scheduler
	Input:			- The clock frequency of the microcontroller					
	Returns:		No
	************************************************************************/
	Scheduler(uint32_t processorFrequensy);

	/***********************************************************************
	Function:		AddTask()
	Purpose:		Add task in the queue
	Input:			- Function to perform	
					- Execution period in milliseconds				
	Returns:		No
	************************************************************************/
	void AddTask(void (*function)(void), uint16_t executionPeriod);

	/***********************************************************************
	Function:		DeleteTask()
	Purpose:		Delete task from the queue
	Input:			- Function to delete					
	Returns:		No
	************************************************************************/	
	void DeleteTask(void (*function)(void));
	
	/***********************************************************************
	Function:		UpdateTimeCounter()
	Purpose:		Increasing the time counter
	Input:			No					
	Returns:		No
	************************************************************************/
	void UpdateLeftTime();

	/***********************************************************************
	Function:		DispatchTasks()
	Purpose:		Processing the task queue
	Input:			No					
	Returns:		No
	************************************************************************/	
	void DispatchTasks();	
	
	private:
	
	uint8_t tasksCounter;
	struct Task tasks[MAX_TASKS];	

	void EnableInterrupt();
	void DisableInterrupt();
};

#endif /* SCHEDULER_H_ */

/***********************************************************************
								HOW IT USE
						
					-=Global interrupts must be allowed=-

sei();

************************************************************************
					-=Create the scheduler object=-		
									
Scheduler scheduler(F_CPU);

************************************************************************						
				-=Define the scheduler interrupt vector=-
						
ISR(SCHEDULER_INTERRUPT)
{
	scheduler.UpdateTimeCounter(); <- Here the time counter increases
}

************************************************************************
						-=Add tasks in queue=-

Function must be static and void without incoming parameters.

void SomeFunc()
{
	***
}

***

scheduler.AddTask(SomeFunc, 200);

***

************************************************************************
						-=Delete tasks from queue=-
						
***

scheduler.DeleteTask(SomeFunc);

***

************************************************************************
						-=Dispatching tasks=-
						
int main(void)
{
	***
	
	sei();
	
	while (1)
	{
		***
		
		scheduler.DispatchTasks();
		
		***
	}
}

************************************************************************/