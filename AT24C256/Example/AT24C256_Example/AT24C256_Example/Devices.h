/************************************************************************************
Title:						Devices list
		 
Notes:						- Buffer for accessing the same type of devices
*************************************************************************************/

#ifndef __DEVICES_H__
#define __DEVICES_H__

#include <avr/io.h>
#include <string.h>

template <typename T>
class Devices
{
	public:
	
	/***********************************************************************
	Function:		Devices()
	Purpose:		Initializing buffer
	Input:			No
	Returns:		No
	************************************************************************/
	Devices();
	
	/***********************************************************************
	Function:		Add()
	Purpose:		Adding device pointer in buffer
	Input:			- device pointer
	Returns:		No
	************************************************************************/
	void Add(T *device);

	/***********************************************************************
	Function:		Next()
	Purpose:		Switching to the next device in the buffer
	Input:			No
	Returns:		No
	************************************************************************/
	void Next();

	/***********************************************************************
	Function:		CurrentDevice()
	Purpose:		Returns the device currently being processed
	Input:			No
	Returns:		Device pointer
	************************************************************************/
	T* CurrentDevice();
	
	~Devices();
	
	void* operator new(size_t size);
		
	void operator delete(void* ptr);
	
	private:
	
	T **devicesList;
	
	uint8_t selectedDevice;
	uint8_t devicesQuantity;
};

template <typename T>
Devices<T>::Devices()
{
	this->selectedDevice = 0;
	this->devicesQuantity = 0;
}

template <typename T>
void Devices<T>::Add(T *device)
{
	this->devicesQuantity++;
		
	uint16_t newSize = this->devicesQuantity * sizeof(T*);
	
	T **ptr = nullptr;		
	do 
	{
		ptr = (T**)realloc(this->devicesList, newSize);
	}while (ptr == nullptr);
	
	this->devicesList = ptr;
	
	this->devicesList[this->devicesQuantity - 1] = device;
}

template <typename T>
void Devices<T>::Next()
{
	if(this->devicesQuantity > 0)
	{
		++this->selectedDevice %= this->devicesQuantity;
	}
}

template <typename T>
T* Devices<T>::CurrentDevice()
{
	return this->devicesList[this->selectedDevice];
}

template <typename T>
Devices<T>::~Devices()
{
	free((T**)this->devicesList);
}

template <typename T>
void* Devices<T>::operator new(size_t size)
{
	void* ptr;
	do
	{
		ptr = malloc(size);
	}while(ptr == nullptr);
	
	return ptr;
}

template <typename T>
void Devices<T>::operator delete(void* ptr)
{
	free(ptr);
}

#endif //__DEVICES_H__
