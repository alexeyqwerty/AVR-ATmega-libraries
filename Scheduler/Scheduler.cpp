#include "Scheduler.h"

 Scheduler::Scheduler(uint32_t processorFrequensy)
 {	 
	 uint16_t millisecondsPerSeconds = 1000;
	 uint8_t timerPrescaler = 64;
	 
	 tasksCounter = 0;
	 	 
	 TCCR |= (1<<WGM01)|(1<<CS01)|(1<<CS00);		//Mode CTC, prescaler 64		
	 _OCR0 = (processorFrequensy / (timerPrescaler * millisecondsPerSeconds));	
	 
	 EnableInterrupt();
 }
 
void Scheduler::AddTask(void (*function)(void), uint16_t executionPeriod)
{	
	if(tasksCounter < MAX_TASKS)
	{
		bool functionExist = false;
		
		for(uint8_t i = 0; i < tasksCounter; i++)
		{
			if(tasks[i].function == function)
			{
				functionExist = true;
				
				tasks[i].executionPeriod = executionPeriod;
				tasks[i].timeLeft = 0;				
				
				break;		
			}
		}
		
		if(!functionExist)
		{
			tasks[tasksCounter].function = function;
			tasks[tasksCounter].executionPeriod = executionPeriod;
			tasks[tasksCounter].timeLeft = 0;
					
			tasksCounter++;
		}
	}
}

void Scheduler::DeleteTask(void (*function)(void))
{		
	if(tasksCounter > 0)
	{		
		uint8_t deletingTask = tasksCounter;
		
		for(uint8_t i = 0; i < tasksCounter; i++)
		{
			if(tasks[i].function == function)
			{
				deletingTask = i;
				break;
			}
		}
		
		if (deletingTask < tasksCounter)
		{
			if(deletingTask < --tasksCounter)
			{
				tasks[deletingTask] = tasks[tasksCounter];
			}
		}
	}
}

void Scheduler::UpdateLeftTime()
{
	for(uint8_t i = 0; i < tasksCounter; i++)
	{
		tasks[i].timeLeft++;
	}
}

void Scheduler::DispatchTasks()
{	
	for(uint8_t i = 0; i < tasksCounter; i++)
	{
		if(tasks[i].timeLeft >= tasks[i].executionPeriod)
		{
			tasks[i].timeLeft = 0;
					
			tasks[i].function();
		}
	}			
}

void Scheduler::EnableInterrupt()
{
	if(TCNT0 >= _OCR0) TCNT0 = 0;		
	
	_TIMSK |= (1<<_OCIE0);	
}

void Scheduler::DisableInterrupt()
{
	_TIMSK &= ~(1<<_OCIE0);
}
