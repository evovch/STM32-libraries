/*
 * hdc1000.c
 *
 *  Created on: May 6, 2019
 *      Author: evovch
 */

#include "hdc1000.h"

#ifdef USELDC
#include <stdio.h> // for sprintf
#include "lcd_pcd8544.h"
#endif // USELDC

HAL_StatusTypeDef HDC1000_Config(HDC1000_t* sensor)
{
	// Configuration - 0x02
	sensor->mTXbuf[0] = 0x02;

	sensor->mTXbuf[1] = ((sensor->mMode & 0x1) << 4);
	sensor->mTXbuf[2] = 0b00000000;

	HAL_StatusTypeDef stat;
	stat = HAL_I2C_Master_Transmit_IT(sensor->mI2cHandle, sensor->mAddress, sensor->mTXbuf, 3);

#ifdef USELDC
	char str[14];
	sprintf(str, "Config:%s", stat==HAL_OK?"OK":"ERR");
	LCD_PCD8544_push_line2(&gLcdScreen, str);
#endif // USELDC
	HAL_Delay(HDC1000_DELAY);

	return stat;
}

//TODO improve error handling and return value
float HDC1000_AcquireTemperature(HDC1000_t* sensor)
{
	HAL_StatusTypeDef stat;

	// Temperature - 0x00
	sensor->mTXbuf[0] = 0x00;

	// Measurement trigger
	stat = HAL_I2C_Master_Transmit_IT(sensor->mI2cHandle, sensor->mAddress, sensor->mTXbuf, 1);
#ifdef USELDC
	char str[14];
	//sprintf(str, "TXt:%s", stat==HAL_OK?"OK":"ERR");
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
#endif // USELDC
	HAL_Delay(HDC1000_DELAY);

	// Acquisition
	stat = HAL_I2C_Master_Receive_IT(sensor->mI2cHandle, sensor->mAddress, sensor->mRXbuf, 2);
	// Conversion
	unsigned short int MSB = (unsigned short int)sensor->mRXbuf[0];
	unsigned short int LSB = (unsigned short int)sensor->mRXbuf[1];
	unsigned short int val = ((MSB << 8) & 0xff00) | (LSB & 0x00ff);
	float fval = (float)val*165.f/65536.f - 40.f;

#ifdef USELDC
	int p1 = (int)fval;
	int p2 = (int)(fval*10.f) % 10;
	//sprintf(str, "RXt:%s:%u/%u", stat==HAL_OK?"OK":"ERR", gHDC1000_I2C_RXbuf[0], gHDC1000_I2C_RXbuf[1]);
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
	//sprintf(str, "RXt:%s:%d.%d", stat==HAL_OK?"OK":"ERR", p1, p2);
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
	sprintf(str, "tmp:%d.%d", p1, p2);
	LCD_PCD8544_push_line2(&gLcdScreen, str);
#endif // USELDC

	HAL_Delay(HDC1000_DELAY);

	return fval;
}

//TODO improve error handling and return value
float HDC1000_AcquireHumidity(HDC1000_t* sensor)
{
	HAL_StatusTypeDef stat;

	// Humidity - 0x01
	sensor->mTXbuf[0] = 0x01;

	// Measurement trigger
	stat = HAL_I2C_Master_Transmit_IT(sensor->mI2cHandle, sensor->mAddress, sensor->mTXbuf, 1);
#ifdef USELDC
	char str[14];
	//sprintf(str, "TXh:%s", stat==HAL_OK?"OK":"ERR");
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
#endif // USELDC
	HAL_Delay(HDC1000_DELAY);

	// Acquisition
	stat = HAL_I2C_Master_Receive_IT(sensor->mI2cHandle, sensor->mAddress, sensor->mRXbuf, 2);
	// Conversion
	unsigned short int MSB = (unsigned short int)sensor->mRXbuf[0];
	unsigned short int LSB = (unsigned short int)sensor->mRXbuf[1];
	unsigned short int val = ((MSB << 8) & 0xff00) | (LSB & 0x00ff);
	float fval = (float)val*100.f/65536.f;

#ifdef USELDC
	int p1 = (int)fval;
	int p2 = (int)(fval*10.f) % 10;
	//sprintf(str, "RXh:%s:%u/%u", stat==HAL_OK?"OK":"ERR", gHDC1000_I2C_RXbuf[0], gHDC1000_I2C_RXbuf[1]);
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
	//sprintf(str, "RXh:%s:%d.%d", stat==HAL_OK?"OK":"ERR", p1, p2);
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
	sprintf(str, "hum:%d.%d", p1, p2);
	LCD_PCD8544_push_line2(&gLcdScreen, str);
#endif // USELDC

	HAL_Delay(HDC1000_DELAY);

	return fval;
}

//TODO improve error handling and return value
void HDC1000_AcquireTempAndHumid(HDC1000_t* sensor, float* t, float* h)
{
	HAL_StatusTypeDef stat;

	// Temperature and humidity - 0x00
	sensor->mTXbuf[0] = 0x00;

	// Measurement trigger
	stat = HAL_I2C_Master_Transmit_IT(sensor->mI2cHandle, sensor->mAddress, sensor->mTXbuf, 1);
#ifdef USELDC
	char str[14];
	//sprintf(str, "TXt:%s", stat==HAL_OK?"OK":"ERR");
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
#endif // USELDC
	HAL_Delay(HDC1000_DELAY);

	// Acquisition
	stat = HAL_I2C_Master_Receive_IT(sensor->mI2cHandle, sensor->mAddress, sensor->mRXbuf, 4);
	// Conversion
	unsigned short int MSB = (unsigned short int)sensor->mRXbuf[0];
	unsigned short int LSB = (unsigned short int)sensor->mRXbuf[1];
	unsigned short int val = ((MSB << 8) & 0xff00) | (LSB & 0x00ff);
	float fvalTemp = (float)val*165.f/65536.f - 40.f;
	MSB = (unsigned short int)sensor->mRXbuf[2];
	LSB = (unsigned short int)sensor->mRXbuf[3];
	val = ((MSB << 8) & 0xff00) | (LSB & 0x00ff);
	float fvalHum = (float)val*100.f/65536.f;

#ifdef USELDC
	int p1Temp = (int)fvalTemp;
	int p2Temp = (int)(fvalTemp*10.f) % 10;
	int p1Hum = (int)fvalHum;
	int p2Hum = (int)(fvalHum*10.f) % 10;
	//sprintf(str, "RXt:%s:%u/%u", stat==HAL_OK?"OK":"ERR", gHDC1000_I2C_RXbuf[0], gHDC1000_I2C_RXbuf[1]);
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
	//sprintf(str, "RXt:%s:%d.%d", stat==HAL_OK?"OK":"ERR", p1, p2);
	//LCD_PCD8544_push_line2(&gLcdScreen, str);
	sprintf(str, "tmp:%d.%d", p1Temp, p2Temp);
	LCD_PCD8544_push_line2(&gLcdScreen, str);
	sprintf(str, "hum:%d.%d", p1Hum, p2Hum);
	LCD_PCD8544_push_line2(&gLcdScreen, str);
#endif // USELDC

	HAL_Delay(HDC1000_DELAY);

	// Return statement
	if (t != NULL) *t = fvalTemp;
	if (h != NULL) *h = fvalHum;
}
