/*
 * hdc1000.h
 *
 *  Created on: May 6, 2019
 *      Author: evovch
 */

#ifndef __HDC1000_H
#define __HDC1000_H

#include "i2c.h"

// Comment/uncomment to disable/enable LCD printout
//#define USELDC

// approximate delay after each I2C Tx/Rx request in ms
#define HDC1000_DELAY 20

typedef enum {
	HDC1000_SINGLE = 0, // either temperature or humidity
	HDC1000_BOTH = 1 // both temperature and humidity
} HDC1000_mode_t;

typedef struct {
	I2C_HandleTypeDef* mI2cHandle;
	uint8_t mAddress;
	HDC1000_mode_t mMode;
	uint8_t mTXbuf[4];
	uint8_t mRXbuf[4];
} HDC1000_t;

// =============================================================================
// Global variables
// =============================================================================

HDC1000_t gHDC1000sensor;

// =============================================================================

HAL_StatusTypeDef HDC1000_Config(HDC1000_t* sensor);

float HDC1000_AcquireTemperature(HDC1000_t* sensor);

float HDC1000_AcquireHumidity(HDC1000_t* sensor);

void HDC1000_AcquireTempAndHumid(HDC1000_t* sensor, float* t, float* h);

// =============================================================================

#endif /* __HDC1000_H */
