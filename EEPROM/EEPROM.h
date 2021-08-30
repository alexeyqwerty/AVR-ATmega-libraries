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

class EEPROM
{
	public:
	/***********************************************************************
	Data type:		int8
	************************************************************************/
	static int8_t GetInt8(const int8_t* dataPointer);
	static bool SetInt8(int8_t* dataPointer, int8_t data);
	
	/***********************************************************************
	Data type:		uint8
	************************************************************************/
	static uint8_t GetUint8(const uint8_t* dataPointer);
	static bool SetUint8(uint8_t* dataPointer, uint8_t data);
	
	/***********************************************************************
	Data type:		int16
	************************************************************************/
	static int16_t GetInt16(const int16_t* dataPointer);
	static bool SetInt16(int16_t* dataPointer, int16_t data);
	
	/***********************************************************************
	Data type:		uint16
	************************************************************************/
	static uint16_t GetUint16(const uint16_t* dataPointer);
	static bool SetUint16(uint16_t* dataPointer, uint16_t data);
	
	/***********************************************************************
	Data type:		int32
	************************************************************************/
	static int32_t GetInt32(const int32_t* dataPointer);
	static bool SetInt32(int32_t* dataPointer, int32_t data);
	
	/***********************************************************************
	Data type:		uint32
	************************************************************************/
	static uint32_t GetUint32(const uint32_t* dataPointer);
	static bool SetUint32(uint32_t* dataPointer, uint32_t data);
	
	/***********************************************************************
	Data type:		float
	************************************************************************/
	static float GetFloat(const float* dataPointer);
	static bool SetFloat(float* dataPointer, float data);
	
	/***********************************************************************
	Data type:		int8[]
	************************************************************************/
	static int8_t GetInt8ArrayElement(const int8_t* dataPointer, uint16_t elementNum);
	static bool SetInt8ArrayElement(int8_t* dataPointer, uint16_t elementNum, int8_t data);
	
	/***********************************************************************
	Data type:		uint8[]
	************************************************************************/
	static uint8_t GetUint8ArrayElement(const uint8_t* dataPointer, uint16_t elementNum);
	static bool SetUint8ArrayElement(uint8_t* dataPointer, uint16_t elementNum, uint8_t data);
	
	/***********************************************************************
	Data type:		int16[]
	************************************************************************/
	static int16_t GetInt16ArrayElement(const int16_t* dataPointer, uint16_t elementNum);
	static bool SetInt16ArrayElement(int16_t* dataPointer, uint16_t elementNum, int16_t data);
	
	/***********************************************************************
	Data type:		uint16[]
	************************************************************************/
	static uint16_t GetUint16ArrayElement(const uint16_t* dataPointer, uint16_t elementNum);
	static bool SetUint16ArrayElement(uint16_t* dataPointer, uint16_t elementNum, uint16_t data);
	
	/***********************************************************************
	Data type:		int32[]
	************************************************************************/
	static int32_t GetInt32ArrayElement(const int32_t* dataPointer, uint16_t elementNum);
	static bool SetInt32ArrayElement(int32_t* dataPointer, uint16_t elementNum, int32_t data);
		
	/***********************************************************************
	Data type:		uint32[]
	************************************************************************/
	static uint32_t GetUint32ArrayElement(const uint32_t* dataPointer, uint16_t elementNum);
	static bool SetUint32ArrayElement(uint32_t* dataPointer, uint16_t elementNum, uint32_t data);
	
	/***********************************************************************
	Data type:		float[]
	************************************************************************/
	static float GetFloatArrayElement(const float* dataPointer, uint16_t elementNum);
	static bool SetFloatArrayElement(float* dataPointer, uint16_t elementNum, float data);
};


#endif /* EEPROM_H_ */