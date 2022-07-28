#include "Scheduler.h"

Scheduler *scheduler = new Scheduler(F_CPU);

 Scheduler::Scheduler(uint32_t processorFrequensy)
 {	 
	 uint16_t millisecondsPerSeconds = 1000;
	 uint8_t timerPrescaler = 64;
	 
	 tasksCounter = 0;
	 selectedTask = 0;
	 	 
	 TCCR |= (1<<WGM01)|(1<<CS01)|(1<<CS00);		//Mode CTC, prescaler 64		
	 _OCR0 = (processorFrequensy / (timerPrescaler * millisecondsPerSeconds));		 
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
				tasks[i].isRun = false;			
				
				break;		
			}
		}
		
		if(!functionExist)
		{
			tasks[tasksCounter].function = function;
			tasks[tasksCounter].executionPeriod = executionPeriod;
			tasks[tasksCounter].timeLeft = 0;
			
			if(tasksCounter++ == 0) EnableDispatch();					
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
		
		if(tasksCounter == 0) DisableDispatch();
	}
}

void Scheduler::UpdateLeftTime()
{
	TCNT0 = 0;
	
	for(uint8_t i = 0; i < tasksCounter; i++)
	{
		if(!tasks[i].isRun)
		{
			tasks[i].timeLeft++;
		}
	}
}

void Scheduler::DispatchTasks()
{		
	if(tasksCounter == 0) return;
	
	if(tasks[selectedTask].timeLeft >= tasks[selectedTask].executionPeriod)
	{
		tasks[selectedTask].isRun = true;			
		tasks[selectedTask].timeLeft = 0;					
		tasks[selectedTask].function();			
		tasks[selectedTask].isRun = false;
	}			
	
	selectedTask = (selectedTask + 1) % tasksCounter;
}

void Scheduler::ClearQueue()
{
	tasksCounter = 0;
	selectedTask = 0;
	DisableDispatch();
}

void Scheduler::EnableDispatch()
{
	TCNT0 = 0;
	_TIMSK |= (1<<_OCIE0);
}

void Scheduler::DisableDispatch()
{
	_TIMSK &= ~(1<<_OCIE0);
}

Scheduler::~Scheduler(){}

void* Scheduler::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

void Scheduler::operator delete(void* ptr)
{
	if(ptr != nullptr) free(ptr);
}