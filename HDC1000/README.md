HDC1000 Temperature and humidity sensor
=======================================

Configure your MCU using STM32CubeMX.

I2C clock speed has to be between 10 and 400 kHz.

[figure1](./STM32CubeMX.png)

Suitable for (tested on) the very popular cheap Chinese boards [STM32F103C8T6 "Blue Pill"](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill), [STM32FxCxT6 Board v5.02](https://stm32-base.org/boards/STM32F103C8T6-STM32FxCxT6-Board-V5.02), and [STM32F407VET6 - STM32 F4VE V2.0](https://stm32-base.org/boards/STM32F407VET6-STM32-F4VE-V2.0).

Example of usage:
-----------------

```

#include "hdc1000.h"

...

  // Temperature and humidity sensor
  extern HDC1000_t gHDC1000sensor;

  // Temperature and humidity sensor - assign I2C interface and address
  gHDC1000sensor.mI2cHandle = &hi2c1;
  gHDC1000sensor.mAddress = 0b10000100;
  gHDC1000sensor.mMode = HDC1000_BOTH;

  // Temperature and humidity sensor - configure
  HDC1000_Config(&gHDC1000sensor);

  // Acquisition
  HDC1000_AcquireTempAndHumid(&gHDC1000sensor, NULL, NULL);

...

```

Example 2:
----------

Can be combined with the LCD_PCD8544.

```
#define USELDC

...

#include "lcd_pcd8544.h"
#include "hdc1000.h"

...

  // LCD
  extern LCD_PCD8544_screen_t gLcdScreen;

  // LCD - assign SPI interface and pins
  gLcdScreen.mPinout.mSpiHandle = &hspi2;
  gLcdScreen.mPinout.mLcdSpiRstPort = LCD_RST_GPIO_Port;
  gLcdScreen.mPinout.mLcdSpiRstPin = LCD_RST_Pin;
  gLcdScreen.mPinout.mLcdSpiCePort = LCD_CE_GPIO_Port;
  gLcdScreen.mPinout.mLcdSpiCePin = LCD_CE_Pin;
  gLcdScreen.mPinout.mLcdSpiDcPort = LCD_DC_GPIO_Port;
  gLcdScreen.mPinout.mLcdSpiDcPin = LCD_DC_Pin;
  gLcdScreen.mPinout.mLcdSpiDinPort = LCD_SPI_DIN_GPIO_Port;
  gLcdScreen.mPinout.mLcdSpiDinPin = LCD_SPI_DIN_Pin;
  gLcdScreen.mPinout.mLcdSpiClkPort = LCD_SPI_CLK_GPIO_Port;
  gLcdScreen.mPinout.mLcdSpiClkPin = LCD_SPI_CLK_Pin;
  gLcdScreen.mPinout.mLcdErrorLedPort = LCD_ERROR_LED_GPIO_Port;
  gLcdScreen.mPinout.mLcdErrorLedPin = LCD_ERROR_LED_Pin;

  // LCD - initialize
  LCD_PCD8544_init(&gLcdScreen);

  // LCD - welcome screen
  LCD_PCD8544_write_line(&gLcdScreen, 0, "Privet!");
  HAL_Delay(1000);

  // Temperature and humidity sensor
  extern HDC1000_t gHDC1000sensor;

  // Temperature and humidity sensor - assign I2C interface and address
  gHDC1000sensor.mI2cHandle = &hi2c1;
  gHDC1000sensor.mAddress = 0b10000100;
  gHDC1000sensor.mMode = HDC1000_BOTH;

  // Temperature and humidity sensor - configure
  HDC1000_Config(&gHDC1000sensor);

  // Acquisition
  HDC1000_AcquireTempAndHumid(&gHDC1000sensor, NULL, NULL);

...

```

