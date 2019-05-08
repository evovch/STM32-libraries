/*
 * mcp4822.c
 *
 *  Created on: May 8, 2019
 *      Author: evovch
 */

#include "mcp4822.h"

void MCP4822_SetValue(MCP4822_DAC_t* dac, enu_MCP4822_CHANNEL ch, unsigned short int val)
{
	unsigned char outData1[2] = {0, 0};

	outData1[1] = ((ch & 0x1) << 7) | ((dac->mGain & 0x1) << 5) | 0b00010000 | ((val >> 8) & 0xf);
	outData1[0] = ((val >> 0) & 0xff);

	//TODO testing - switched from IT to non-IT call
	// added CS pins reset calls (to '1') afterwards
	// last argument - timeout in ms
	HAL_StatusTypeDef stat;
	HAL_GPIO_WritePin(dac->mPinout.mSpiCsPort, dac->mPinout.mSpiCsPin, GPIO_PIN_RESET);
	stat = HAL_SPI_Transmit(dac->mPinout.mSpiHandle, outData1, 1, MCP4822_SPI_TIMEOUT); // HAL_SPI_Transmit_IT
	HAL_GPIO_WritePin(dac->mPinout.mSpiCsPort, dac->mPinout.mSpiCsPin, GPIO_PIN_SET);

#ifdef MCP4822_ENABLE_LCD
	if (stat != HAL_OK) {
		extern LCD_PCD8544_screen_t gLcdScreen;
		LCD_PCD8544_write_str(&gLcdScreen, 5, 2, "DAC SPI ERR");
	}
#endif // MCP4822_ENABLE_LCD
}

void MCP4822_SetValues(MCP4822_DAC_t* dac, unsigned short int val_chA, unsigned short int val_chB)
{
	unsigned char outDataA[2] = {0, 0};
	unsigned char outDataB[2] = {0, 0};

	outDataA[1] = ((MCP4822_CHANNEL_A & 0x1) << 7) | ((dac->mGain & 0x1) << 5) | 0b00010000 | ((val_chA >> 8) & 0xf);
	outDataA[0] = ((val_chA >> 0) & 0xff);
	outDataB[1] = ((MCP4822_CHANNEL_B & 0x1) << 7) | ((dac->mGain & 0x1) << 5) | 0b00010000 | ((val_chB >> 8) & 0xf);
	outDataB[0] = ((val_chB >> 0) & 0xff);

	//TODO testing - switched from IT to non-IT call
	// added CS pins reset calls (to '1') afterwards
	// last argument - timeout in ms
	HAL_StatusTypeDef statA;
	HAL_StatusTypeDef statB;
	HAL_GPIO_WritePin(dac->mPinout.mSpiCsPort, dac->mPinout.mSpiCsPin, GPIO_PIN_RESET);
	statA = HAL_SPI_Transmit(dac->mPinout.mSpiHandle, outDataA, 1, MCP4822_SPI_TIMEOUT); // HAL_SPI_Transmit_IT
	HAL_GPIO_WritePin(dac->mPinout.mSpiCsPort, dac->mPinout.mSpiCsPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(dac->mPinout.mSpiCsPort, dac->mPinout.mSpiCsPin, GPIO_PIN_RESET);
	statB = HAL_SPI_Transmit(dac->mPinout.mSpiHandle, outDataB, 1, MCP4822_SPI_TIMEOUT); // HAL_SPI_Transmit_IT
	HAL_GPIO_WritePin(dac->mPinout.mSpiCsPort, dac->mPinout.mSpiCsPin, GPIO_PIN_SET);

#ifdef MCP4822_ENABLE_LCD
	if ((statA != HAL_OK) || (statB != HAL_OK)) {
		extern LCD_PCD8544_screen_t gLcdScreen;
		LCD_PCD8544_write_str(&gLcdScreen, 5, 2, "DAC SPI ERR");
	}
#endif // MCP4822_ENABLE_LCD
}
