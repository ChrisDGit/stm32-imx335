/**
  ******************************************************************************
  * @file    imx219.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the imx219.c
  *          driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IMX219_H
#define IMX219_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "imx219_reg.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup imx219
  * @{
  */

/** @defgroup IMX219_Exported_Types
  * @{
  */

typedef int32_t (*IMX219_Init_Func)    (void);
typedef int32_t (*IMX219_DeInit_Func)  (void);
typedef int32_t (*IMX219_GetTick_Func) (void);
typedef int32_t (*IMX219_WriteReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*IMX219_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  IMX219_Init_Func          Init;
  IMX219_DeInit_Func        DeInit;
  uint16_t                  Address;
  IMX219_WriteReg_Func      WriteReg;
  IMX219_ReadReg_Func       ReadReg;
  IMX219_GetTick_Func       GetTick;
} IMX219_IO_t;

typedef struct
{
  IMX219_IO_t         IO;
  imx219_ctx_t        Ctx;
  uint8_t             IsInitialized;
} IMX219_Object_t;

typedef struct
{
  uint32_t Config_Resolution;
  uint32_t Config_LightMode;
  uint32_t Config_SpecialEffect;
  uint32_t Config_Brightness;
  uint32_t Config_Saturation;
  uint32_t Config_Contrast;
  uint32_t Config_HueDegree;
  uint32_t Config_Gain;
  uint32_t Config_Exposure;
  uint32_t Config_MirrorFlip;
  uint32_t Config_Zoom;
  uint32_t Config_NightMode;
  uint32_t Config_ExposureMode;
  uint32_t Config_SensorInfo;
  uint32_t Config_TestPattern;
  uint32_t Config_ColorEffect;
} IMX219_Capabilities_t;

#define IMX219_SENSOR_INFO_MAX_LENGTH      (32U)

typedef struct
{
  char name[IMX219_SENSOR_INFO_MAX_LENGTH];
  uint8_t bayer_pattern;
  uint8_t color_depth;
  uint32_t width;
  uint32_t height;
  uint32_t gain_min;
  uint32_t gain_max;
  uint32_t exposure_min;
  uint32_t exposure_max;
} IMX219_SensorInfo_t;

typedef struct
{
  int32_t  (*Init              )(IMX219_Object_t*, uint32_t, uint32_t);
  int32_t  (*DeInit            )(IMX219_Object_t*);
  int32_t  (*ReadID            )(IMX219_Object_t*, uint32_t*);
  int32_t  (*GetCapabilities   )(IMX219_Object_t*, IMX219_Capabilities_t*);
  int32_t  (*SetLightMode      )(IMX219_Object_t*, uint32_t);
  int32_t  (*SetColorEffect    )(IMX219_Object_t*, uint32_t);
  int32_t  (*SetBrightness     )(IMX219_Object_t*, int32_t);
  int32_t  (*SetSaturation     )(IMX219_Object_t*, int32_t);
  int32_t  (*SetContrast       )(IMX219_Object_t*, int32_t);
  int32_t  (*SetHueDegree      )(IMX219_Object_t*, int32_t);
  int32_t  (*MirrorFlipConfig  )(IMX219_Object_t*, uint32_t);
  int32_t  (*ZoomConfig        )(IMX219_Object_t*, uint32_t);
  int32_t  (*SetResolution     )(IMX219_Object_t*, uint32_t);
  int32_t  (*GetResolution     )(IMX219_Object_t*, uint32_t*);
  int32_t  (*SetPixelFormat    )(IMX219_Object_t*, uint32_t);
  int32_t  (*GetPixelFormat    )(IMX219_Object_t*, uint32_t*);
  int32_t  (*NightModeConfig   )(IMX219_Object_t*, uint32_t);
  int32_t  (*SetFrequency      )(IMX219_Object_t*, int32_t);
  int32_t  (*SetGain           )(IMX219_Object_t*, int32_t);
  int32_t  (*SetExposure       )(IMX219_Object_t*, int32_t);
  int32_t  (*SetExposureMode   )(IMX219_Object_t*, int32_t);
  int32_t  (*GetSensorInfo     )(IMX219_Object_t *, IMX219_SensorInfo_t *);
  int32_t  (*SetTestPattern    )(IMX219_Object_t*, int32_t);
}IMX219_CAMERA_Drv_t;
/**
  * @}
  */

/** @defgroup IMX219_Exported_Constants
  * @{
  */
#define IMX219_OK                      (0)
#define IMX219_ERROR                   (-1)
/**
 * @brief  IMX219 Features Parameters
 */
/* Camera resolutions */
#define IMX219_R3280_2464              0U    /* 3280x2464 Resolution (8MP, 15fps) */
#define IMX219_R1920_1080              1U    /* 1920x1080 Resolution (1080p, 30fps) */
#define IMX219_R1640_1232              2U    /* 1640x1232 Resolution (2x2 binned, 30fps) */
#define IMX219_R640_480                3U    /* 640x480 Resolution (30fps) */

/* Camera Pixel Format */
#define IMX219_RAW_RGGB8               8U    /* Pixel Format RAW_RGGB8     */
#define IMX219_RAW_RGGB10              10U   /* Pixel Format RAW_RGGB10    */

/* Camera Frequency */
#define IMX219_INCK_24MHZ              0U    /* 24MHz external clock */

/* Test Pattern */
#define IMX219_TEST_PATTERN_DISABLED   0U    /* Test pattern disabled */
#define IMX219_TEST_PATTERN_SOLID      1U    /* Solid color test pattern */
#define IMX219_TEST_PATTERN_COLOR_BARS 2U    /* Color bars test pattern */
#define IMX219_TEST_PATTERN_GREY_BARS  3U    /* Grey bars test pattern */
#define IMX219_TEST_PATTERN_PN9        4U    /* PN9 test pattern */

/* Mirror/Flip */
#define IMX219_MIRROR_FLIP_NONE        0x00U /* Set camera normal mode     */
#define IMX219_FLIP                    0x01U /* Set camera flip config     */
#define IMX219_MIRROR                  0x02U /* Set camera mirror config   */
#define IMX219_MIRROR_FLIP             0x03U /* Set camera mirror and flip */

/**
  * @}
  */

/** @defgroup IMX219_Exported_Functions IMX219 Exported Functions
  * @{
  */
int32_t IMX219_RegisterBusIO(IMX219_Object_t *pObj, IMX219_IO_t *pIO);
int32_t IMX219_Init(IMX219_Object_t *pObj, uint32_t Resolution, uint32_t PixelFormat);
int32_t IMX219_DeInit(IMX219_Object_t *pObj);
int32_t IMX219_ReadID(IMX219_Object_t *pObj, uint32_t *Id);
int32_t IMX219_GetCapabilities(IMX219_Object_t *pObj, IMX219_Capabilities_t *Capabilities);
int32_t IMX219_SetGain(IMX219_Object_t *pObj, int32_t gain);
int32_t IMX219_SetExposure(IMX219_Object_t *pObj, int32_t exposure);
int32_t IMX219_SetFrequency(IMX219_Object_t *pObj, int32_t frequency);
int32_t IMX219_SetFramerate(IMX219_Object_t *pObj, int32_t framerate);
int32_t IMX219_MirrorFlipConfig(IMX219_Object_t *pObj, uint32_t Config);
int32_t IMX219_GetSensorInfo(IMX219_Object_t *pObj, IMX219_SensorInfo_t *Info);
int32_t IMX219_SetTestPattern(IMX219_Object_t *pObj, int32_t mode);

/* CAMERA driver structure */
extern IMX219_CAMERA_Drv_t   IMX219_CAMERA_Driver;
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */