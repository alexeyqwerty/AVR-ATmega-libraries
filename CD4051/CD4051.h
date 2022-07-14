/************************************************************************************
Title:						Library for handling with the CD4051 multiplexer
		 
Notes:						- Disabling the outputs puts it in the Z state
							- By default, channel 0 is selected
*************************************************************************************/

#ifndef CD4051_H_
#define CD4051_H_

#include <avr/io.h>
#include <stdlib.h>
#include "Port.h"

class CD4051
{
	public:

	/***********************************************************************
	Function:		CD4051()
	Purpose:		Initializing the multiplexer input handler
	Input:			- A-, B- C- and INH- ports 
	Returns:		No
	************************************************************************/
	CD4051(Port* A, Port* B, Port* C, Port* INH);

	/***********************************************************************
	Function:		SetChannel()
	Purpose:		Output selection
	Input:			- Channel number
	Returns:		No
	************************************************************************/			
	void SelectChannel(uint8_t channelNumber);
	
	/***********************************************************************
	Functions:		EnableOutputs()
					DisableOutputs()
	Purpose:		Enable and disable multiplexer outputs
	Input:			No
	Returns:		No
	************************************************************************/		
	void EnableOutputs();	
	void DisableOutputs();		
	
	~CD4051();
	
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
	
	private:
	
	Port* A;
	Port* B;
	Port* C;
	Port* INH;
};

#endif /* CD4051_H_ */