/*
 * mcp4822.h
 *
 *  Created on: Apr 24, 2019
 *      Author: evovch
 */

#ifndef __MCP4822_H
#define __MCP4822_H

#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	MCP4822_CHANNEL_A = 0,
	MCP4822_CHANNEL_B = 1
} enu_MCP4822_CHANNEL;

/**
 * hspiDAC: SPI handle
 * val: 0-4095
 */
void MCP4822_SetValue(SPI_HandleTypeDef* hspiDAC, enu_MCP4822_CHANNEL ch, unsigned short int val)
{
	unsigned char outData1[2] = {0, 0};

	// GAIN bit
	// 0: 2x, i.e. Vout = 2*Vref*val/4096
	// 1: 1x, i.e. Vout =   Vref*val/4096, where internal Vref=2.048V
	unsigned char gain = 0;

	outData1[1] = ((ch & 0x1) << 7) | ((gain & 0x1) << 5) | 0b00010000 | ((val >> 8) & 0xf);
	outData1[0] = ((val >> 0) & 0xff);

	//TODO testing - switched from IT to non-IT call
	// added CS pins reset calls (to '1') afterwards
	// last argument - timeout in ms
	HAL_SPI_Transmit(hspiDAC, outData1, 1, 1); // HAL_SPI_Transmit_IT
}

#ifdef __cplusplus
}
#endif

#endif /* __MCP4822_H */
