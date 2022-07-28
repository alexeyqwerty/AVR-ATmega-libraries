/************************************************************************************
Title:						Wrapper library data interpreter for working with EEPROM

Notes:						All Get- functions returns data from EEPROM according with
							data type. All Set- functions are setting data in EEPROM 
							and returns boolean value, true - if writing successfully				
*************************************************************************************/

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stddef.h>

class EEPROM
{
	public:
	
	/***********************************************************************	
	Functions:		Read()
	Purpose:		Reading data from EEPROM
	Input:			- data address
					- data
					- data size
	Returns:		No
	************************************************************************/
	template<typename T>
	static void Read(uint8_t *address, T &data, uint8_t dataSize);
	
	/***********************************************************************	
	Functions:		Write()
	Purpose:		Writing data to EEPROM
	Input:			- data address
					- data
					- data size
	Returns:		No
	************************************************************************/
	template<typename T>
	static void Write(uint8_t *address, const T &data, uint8_t dataSize);
};

template<typename T>
void EEPROM::Read(uint8_t *address, T &data, uint8_t dataSize)
{
	uint8_t *dataPointer = (uint8_t*)&data;
	
	for(uint8_t i = 0; i < dataSize; i++, address++, dataPointer++)
	{
		eeprom_busy_wait();
		
		*dataPointer = eeprom_read_byte(address);
	}	
}

template<typename T>
void EEPROM::Write(uint8_t *address, const T &data, uint8_t dataSize)
{
	const uint8_t *dataPointer = (const uint8_t*)&data;
	
	for(uint8_t i = 0; i < dataSize; i++, address++, dataPointer++)
	{
		eeprom_busy_wait();
		
		eeprom_update_byte(address, *dataPointer);
	}
}

/************************************************************************
							HOW IT USE  

- Simple data types (uint8_t, float, bool, etc. -> T):

T EEMEM ee_t = 0;

T t = 0;

EEPROM::Read((uint8_t*)&ee_t, t, sizeof(t)); - to read

EEPROM::Write((uint8_t*)&ee_t, t, sizeof(t)); - to write

		*		*		*		*		*

- Arrays:

T EEMEM ee_arr[] = {T1, T2, ...};

T arr[] = {T1, T2, ...};

EEPROM::Read((uint8_t*)&ee_arr, arr, sizeof(arr)); - to read

EEPROM::Write((uint8_t*)&ee_arr, arr, sizeof(arr)); - to write

		*		*		*		*		*

- Structs:

Struct SomeStruct
{
	T t;
	M m[];	
};

Struct SomeStruct EEMEM ee_str = {T, {M1, M2,...}};

Struct SomeStruct str = {T, {M1, M2,...}};

EEPROM::Read((uint8_t*)&ee_str, str, sizeof(str)); - to read

EEPROM::Write((uint8_t*)&ee_str, str, sizeof(str)); - to write

*************************************************************************/

#endif /* EEPROM_H_ */