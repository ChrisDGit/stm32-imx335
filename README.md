# BSP STM32 Camera Sensor Components

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/stm32-imx335.svg?color=brightgreen)

## Overview

**STM32Cube** is an STMicroelectronics original initiative to ease developers' life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform delivered for each STM32 series.
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
   * The BSP drivers of each evaluation, discovery or nucleo board provided for this STM32 series.
   * A consistent set of middleware libraries such as RTOS, USB, File System, Graphics, Touch Sensing library...
   * A full set of software projects (basic examples, applications, and demonstrations) for each board provided for this STM32 series.

Two models of publication are proposed for the STM32Cube embedded software:
   * The monolithic **MCU Package**: all STM32Cube software modules of one STM32 series are present (Drivers, Middleware, Projects, Utilities) in the repository (usual name **STM32Cubexx**, xx corresponding to the STM32 series).
   * The **MCU component**: each STM32Cube software module being part of the STM32Cube MCU Package, is delivered as an individual repository, allowing the user to select and get only the required software functions.

## Description

This repository provides STM32Cube BSP camera sensor components for multiple Sony image sensors:

### IMX335 Camera Sensor
- **IMX335** BSP camera sensor component
- Resolution: 2592x1944 (5MP)
- Pixel Format: RAW10
- Interface: I2C
- External Clock: 6MHz/18MHz/24MHz/27MHz/74MHz support

### IMX219 Camera Sensor
- **IMX219** BSP camera sensor component  
- Multiple resolution support:
  - 3280x2464 (8MP, 15fps)
  - 1920x1080 (1080p, 30fps)
  - 1640x1232 (2x2 binned, 30fps)
  - 640x480 (VGA, 30fps)
- Pixel Formats: RAW8, RAW10
- Interface: I2C
- External Clock: 24MHz
- Features: Exposure control, gain control, test patterns, mirror/flip

## API Documentation

Both sensor drivers follow the same STM32Cube BSP component interface:

### Common Functions
- `RegisterBusIO()` - Register I2C bus operations
- `Init()` - Initialize sensor with resolution and pixel format
- `DeInit()` - Deinitialize sensor
- `ReadID()` - Read sensor chip ID
- `GetCapabilities()` - Get sensor capabilities
- `SetGain()` - Set sensor gain
- `SetExposure()` - Set sensor exposure
- `SetTestPattern()` - Configure test pattern
- `MirrorFlipConfig()` - Configure mirror/flip
- `GetSensorInfo()` - Get sensor information

### Usage Example
```c
#include "imx219.h"

IMX219_Object_t imx219_obj;
IMX219_IO_t imx219_io = {
    .Init = BSP_I2C_Init,
    .DeInit = BSP_I2C_DeInit,
    .Address = 0x10,
    .WriteReg = BSP_I2C_WriteReg,
    .ReadReg = BSP_I2C_ReadReg,
    .GetTick = BSP_GetTick
};

/* Register bus operations */
IMX219_RegisterBusIO(&imx219_obj, &imx219_io);

/* Initialize with 1920x1080 resolution and RAW10 format */
IMX219_Init(&imx219_obj, IMX219_R1920_1080, IMX219_RAW_RGGB10);

/* Set gain and exposure */
IMX219_SetGain(&imx219_obj, 20000);  /* 20.0x gain */
IMX219_SetExposure(&imx219_obj, 1000);  /* 1000 lines */
```

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/stm32-imx335/blob/main/Release_Notes.html).

## Compatibility information

Please refer to the repository of the BSP **board** driver you are using to know which version of this BSP component driver to use. It is **crucial** that you use a consistent set of versions.

## Troubleshooting

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
