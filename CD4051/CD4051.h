/************************************************************************************
Title:						Library for handling with the CD4051 multiplexer
		 
Notes:						- Two classes for parallel connection of chips. The CD4051
							  class for processing outputs, is a common class for all 
							  chips. And the CD4051_Inhibit class for processing inputs,
							  a separate instance of the class for each chip
							- Disabling the outputs puts it in the Z state
							- By default, channel 0 is selected
*************************************************************************************/

#ifndef CD4051_H_
#define CD4051_H_

#include <avr/io.h>
#include "Port.h"

class CD4051
{
	public:

	/***********************************************************************
	Function:		CD4051()
	Purpose:		Initializing the multiplexer input handler
	Input:			- A-, B- and C- ports 
	Returns:		No
	************************************************************************/
	CD4051(Port* A, Port* B, Port* C);

	/***********************************************************************
	Function:		SetChannel()
	Purpose:		Output selection
	Input:			- Channel number
	Returns:		No
	************************************************************************/			
	void SelectChannel(uint8_t channelNumber);
			
	private:
	
	Port* A;
	Port* B;
	Port* C;
	
	void SelectChannel_0();		
	void SelectChannel_1();		
	void SelectChannel_2();		
	void SelectChannel_3();		
	void SelectChannel_4();		
	void SelectChannel_5();		
	void SelectChannel_6();		
	void SelectChannel_7();
};

class CD4051_Inhibit
{
	public:

	/***********************************************************************
	Function:		CD4051_Inhibit()
	Purpose:		Initializing the multiplexer output handler
	Input:			- Inhibit port pointer
					- Inhibit DDR pointer
					- Inhibit pin number
	Returns:		No
	************************************************************************/
	CD4051_Inhibit(Port* INH);

	/***********************************************************************
	Functions:		EnableOutputs()
					DisableOutputs()
	Purpose:		Enable and disable multiplexer outputs
	Input:			No
	Returns:		No
	************************************************************************/		
	void EnableOutputs();	
	void DisableOutputs();
	
	private:
	
	Port* INH;
};

#endif /* CD4051_H_ */