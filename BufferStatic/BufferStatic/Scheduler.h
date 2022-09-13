/************************************************************************************
Title:						Task scheduler

Support:					- ATmega16, Atmega32
							- ATmega328
							- ATmega644, ATmega1284							
		 
Notes:						- Using timer T0
							- Minimal time unit - 1ms
							- Cannot be used for time counting							
*************************************************************************************/

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#include "SchedulerMC.h"

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
	Scheduler(uint32_t processorFrequensy, uint8_t bufferSize);

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
	
	/***********************************************************************
	Function:		ClearQueue()
	Purpose:		Remove all tasks
	Input:			No					
	Returns:		No
	************************************************************************/	
	void ClearQueue();	
	
	~Scheduler();	
	
	void* operator new(size_t size);
	
	void operator delete(void* ptr);
	
	private:
	
	void EnableDispatch();
	void DisableDispatch();
	
	uint8_t tasksCounter;
	uint8_t selectedTask;
	
	uint8_t bufferSize;
	
	struct Task **tasks;		
};

#endif /* SCHEDULER_H_ */

/***********************************************************************
								HOW IT USE
						
					-=Global interrupts must be allowed=-

sei();

************************************************************************
					-=Create the scheduler object=-		
									
Scheduler *scheduler = new Scheduler(F_CPU);

************************************************************************						
				-=Define the scheduler interrupt vector=-
						
ISR(SCHEDULER_INTERRUPT)
{
	scheduler->UpdateLeftTime(); <- Here the time counter increases
}

************************************************************************
						-=Add tasks in queue=-

Function must be static and void without incoming parameters.

void SomeFunc()
{
	***
}

***

scheduler->AddTask(SomeFunc, 200);

***

************************************************************************
						-=Delete tasks from queue=-
						
***

scheduler->DeleteTask(SomeFunc);

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
		
		scheduler->DispatchTasks();
		
		***
	}
}

************************************************************************/