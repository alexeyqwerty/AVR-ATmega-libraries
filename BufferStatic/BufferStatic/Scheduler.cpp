#include "Scheduler.h"

 Scheduler::Scheduler(uint32_t processorFrequensy, uint8_t bufferSize)
 {	 	 
	 uint16_t millisecondsPerSeconds = 1000;
	 uint8_t maxOCR0 = 255;
	 
	 uint32_t timerPrescaler = processorFrequensy;
	 timerPrescaler /= maxOCR0;
	 timerPrescaler /= millisecondsPerSeconds;
	 
	 if(timerPrescaler < 8)
	 {
		 timerPrescaler = 8;
		 
		 _TCCR0 |= (1<<CS01);	
	 }
	 
	 else if(timerPrescaler < 64)
	 {
		 timerPrescaler = 64;
		 
		 _TCCR0 |= (1<<CS01)|(1<<CS00);		
	 }
	 
	 else if(timerPrescaler < 256)
	 {
		 timerPrescaler = 256;
		 
		 _TCCR0 |= (1<<CS02);
	 }
	 
	 else if(timerPrescaler < 1024)
	 {
		 timerPrescaler = 1024;
		 
		 _TCCR0 |= (1<<CS02)|(1<<CS00);	
	 }
	 
	 _TCCR0 |= (1<<WGM01);	//Mode CTC
	 _OCR0 = (processorFrequensy / (timerPrescaler * millisecondsPerSeconds));		 
	 
	 this->tasksCounter = 0;
	 this->selectedTask = 0;
	 
	 this->bufferSize = bufferSize;
	 
	 do
		this->tasks = (struct Task**)malloc(this->bufferSize * sizeof(struct Task*));
	 while(this->tasks == nullptr);
	 
	 for(uint8_t i = 0; i < this->bufferSize; i++)
	 {
		 do
			this->tasks[i] = (struct Task*)malloc(sizeof(struct Task));
		while(this->tasks[i] == nullptr);
	 }
 }
 
void Scheduler::AddTask(void (*function)(void), uint16_t executionPeriod)
{	
	if(this->tasksCounter == this->bufferSize) return;
	
	bool functionExist = false;
		
	for(uint8_t i = 0; i < this->tasksCounter; i++)
	{
		if(this->tasks[i]->function == function)
		{
			functionExist = true;
				
			this->tasks[i]->executionPeriod = executionPeriod;
			this->tasks[i]->timeLeft = 0;					
				
			break;		
		}
	}
		
	if(!functionExist)
	{					
		struct Task task;	
			
		task.function = function;
		task.executionPeriod = executionPeriod;
		task.timeLeft = 0;		
		
		memcpy(this->tasks[this->tasksCounter], &task, sizeof(struct Task));		
						
		if(this->tasksCounter == 0) EnableDispatch();	
		
		this->tasksCounter++;				
	}	
}

void Scheduler::DeleteTask(void (*function)(void))
{		
	if(this->tasksCounter == 0) return;

	for(uint8_t i = 0; i < this->tasksCounter; i++)
	{
		if(this->tasks[i]->function == function)
		{
			this->tasksCounter--;
			
			memcpy(this->tasks[i], this->tasks[this->tasksCounter], sizeof(struct Task));
											
			break;
		}
	}	
	
	this->selectedTask = 0;
		
	if(this->tasksCounter == 0) DisableDispatch();	
}

void Scheduler::UpdateLeftTime()
{
	TCNT0 = 0;
	
	for(uint8_t i = 0; i < this->tasksCounter; i++)
	{
		this->tasks[i]->timeLeft++;		
	}
}

void Scheduler::DispatchTasks()
{		
	if(this->tasksCounter == 0) return;
	
	if(this->tasks[this->selectedTask]->timeLeft >= this->tasks[this->selectedTask]->executionPeriod)
	{				
		this->tasks[this->selectedTask]->timeLeft -= this->tasks[this->selectedTask]->executionPeriod;					
		this->tasks[this->selectedTask]->function();				
	}			
	
	++this->selectedTask %= this->tasksCounter;
}

void Scheduler::ClearQueue()
{
	DisableDispatch();
	
	for(uint8_t i = 0; i < this->tasksCounter; i++)
	{
		free(this->tasks[i]);
		this->tasks[i] = nullptr;
	}
	
	free(this->tasks);
	this->tasks = nullptr;
	
	this->tasksCounter = 0;
	this->selectedTask = 0;	
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

Scheduler::~Scheduler()
{
	ClearQueue();
}

void* Scheduler::operator new(size_t size)
{
	void* ptr;
		
	do
		ptr = malloc(size);
	while(ptr == nullptr);
		
	return ptr;
}

void Scheduler::operator delete(void* ptr)
{
	free(ptr);
	ptr = nullptr;
}