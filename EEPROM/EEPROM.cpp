#include "EEPROM.h"

int8_t EEPROM::GetInt8(const int8_t* dataPointer)
{
	eeprom_busy_wait();
	return (int8_t)eeprom_read_byte((uint8_t*)dataPointer);
}

bool EEPROM::SetInt8(int8_t* dataPointer, int8_t data)
{
	eeprom_busy_wait();
	eeprom_update_byte((uint8_t*)dataPointer, (uint8_t)data);
	
	return (data == GetInt8(dataPointer));
}

uint8_t EEPROM::GetUint8(const uint8_t* dataPointer)
{
	eeprom_busy_wait();
	return eeprom_read_byte(dataPointer);
}

bool EEPROM::SetUint8(uint8_t* dataPointer, uint8_t data)
{
	eeprom_busy_wait();
	eeprom_update_byte(dataPointer, data);
	
	return (data == GetUint8(dataPointer));
}

int16_t EEPROM::GetInt16(const int16_t* dataPointer)
{
	eeprom_busy_wait();
	return (int16_t)eeprom_read_word((uint16_t*)dataPointer);
}

bool EEPROM::SetInt16(int16_t* dataPointer, int16_t data)
{
	eeprom_busy_wait();
	eeprom_update_word((uint16_t*)dataPointer, (uint16_t)data);
	
	return (data == GetInt16(dataPointer));
}

uint16_t EEPROM::GetUint16(const uint16_t* dataPointer)
{
	eeprom_busy_wait();
	return eeprom_read_word(dataPointer);
}

bool EEPROM::SetUint16(uint16_t* dataPointer, uint16_t data)
{
	eeprom_busy_wait();
	eeprom_update_word(dataPointer, data);
	
	return (data == GetUint16(dataPointer));
}

int32_t EEPROM::GetInt32(const int32_t* dataPointer)
{
	eeprom_busy_wait();
	return (int32_t)eeprom_read_dword((uint32_t*)dataPointer);
}

bool EEPROM::SetInt32(int32_t* dataPointer, int32_t data)
{
	eeprom_busy_wait();
	eeprom_update_dword((uint32_t*)dataPointer, (uint32_t)data);
	
	return (data == GetInt32(dataPointer));
}

uint32_t EEPROM::GetUint32(const uint32_t* dataPointer)
{
	eeprom_busy_wait();
	return eeprom_read_dword(dataPointer);
}

bool EEPROM::SetUint32(uint32_t* dataPointer, uint32_t data)
{
	eeprom_busy_wait();
	eeprom_update_dword(dataPointer, data);
	
	return (data == GetUint32(dataPointer));
}

float EEPROM::GetFloat(const float* dataPointer)
{
	eeprom_busy_wait();
	return eeprom_read_float(dataPointer);
}

bool EEPROM::SetFloat(float* dataPointer, float data)
{
	eeprom_busy_wait();
	eeprom_update_float(dataPointer, data);
	
	return (data == GetFloat(dataPointer));
}

int8_t EEPROM::GetInt8ArrayElement(const int8_t* dataPointer, uint16_t elementNum)
{
	return GetInt8(&dataPointer[elementNum]);
}

bool EEPROM::SetInt8ArrayElement(int8_t* dataPointer, uint16_t elementNum, int8_t data)
{
	return SetInt8(&dataPointer[elementNum], data);
}

uint8_t EEPROM::GetUint8ArrayElement(const uint8_t* dataPointer, uint16_t elementNum)
{
	return GetUint8(&dataPointer[elementNum]);
}

bool EEPROM::SetUint8ArrayElement(uint8_t* dataPointer, uint16_t elementNum, uint8_t data)
{
	return SetUint8(&dataPointer[elementNum], data);
}

int16_t EEPROM::GetInt16ArrayElement(const int16_t* dataPointer, uint16_t elementNum)
{
	return GetInt16(&dataPointer[elementNum]);
}

bool EEPROM::SetInt16ArrayElement(int16_t* dataPointer, uint16_t elementNum, int16_t data)
{
	return SetInt16(&dataPointer[elementNum], data);
}

uint16_t EEPROM::GetUint16ArrayElement(const uint16_t* dataPointer, uint16_t elementNum)
{
	return GetUint16(&dataPointer[elementNum]);
}

bool EEPROM::SetUint16ArrayElement(uint16_t* dataPointer, uint16_t elementNum, uint16_t data)
{
	return SetUint16(&dataPointer[elementNum], data);
}

int32_t EEPROM::GetInt32ArrayElement(const int32_t* dataPointer, uint16_t elementNum)
{
	return GetInt32(&dataPointer[elementNum]);
}

bool EEPROM::SetInt32ArrayElement(int32_t* dataPointer, uint16_t elementNum, int32_t data)
{
	return SetInt32(&dataPointer[elementNum], data);
}

uint32_t EEPROM::GetUint32ArrayElement(const uint32_t* dataPointer, uint16_t elementNum)
{
	return GetUint32(&dataPointer[elementNum]);
}

bool EEPROM::SetUint32ArrayElement(uint32_t* dataPointer, uint16_t elementNum, uint32_t data)
{
	return SetUint32(&dataPointer[elementNum], data);
}

float EEPROM::GetFloatArrayElement(const float* dataPointer, uint16_t elementNum)
{
	return GetFloat(&dataPointer[elementNum]);
}

bool EEPROM::SetFloatArrayElement(float* dataPointer, uint16_t elementNum, float data)
{
	return SetFloat(&dataPointer[elementNum], data);
}
