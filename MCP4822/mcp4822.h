/*
 * mcp4822.h
 *
 *  Created on: Apr 24, 2019
 *      Author: evovch
 *  Updated on: May 8, 2019
 *
 */

#ifndef __MCP4822_H
#define __MCP4822_H

#include "spi.h"

#define MCP4822_SPI_TIMEOUT 1

//#define MCP4822_ENABLE_LCD

#ifdef MCP4822_ENABLE_LCD
#include "lcd_pcd8544.h"
#endif // MCP4822_ENABLE_LCD

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	MCP4822_CHANNEL_A = 0,
	MCP4822_CHANNEL_B = 1
} enu_MCP4822_CHANNEL;

typedef struct {
	SPI_HandleTypeDef* mSpiHandle;

	GPIO_TypeDef* mSpiCsPort;
	uint16_t mSpiCsPin;

} MCP4822_DAC_Pinout_t;

typedef struct {
	MCP4822_DAC_Pinout_t mPinout;
	// GAIN bit
	// 0: 2x, i.e. Vout = 2*Vref*val/4096
	// 1: 1x, i.e. Vout =   Vref*val/4096, where internal Vref=2.048V
	unsigned char mGain;

} MCP4822_DAC_t;

/**
 * val: 0-4095
 */
void MCP4822_SetValue(MCP4822_DAC_t* dac, enu_MCP4822_CHANNEL ch, unsigned short int val);

/**
 * valA and valB: 0-4095
 */
void MCP4822_SetValues(MCP4822_DAC_t* dac, unsigned short int val_chA, unsigned short int val_chB);

#ifdef __cplusplus
}
#endif

#endif /* __MCP4822_H */
