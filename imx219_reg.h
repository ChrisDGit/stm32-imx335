/**
  ******************************************************************************
  * @file    imx219_reg.h
  * @author  MCD Application Team
  * @brief   Header of imx219_reg.c
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IMX219_REG_H
#define IMX219_REG_H

#include <cmsis_compiler.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup IMX219
  * @{
  */

/** @defgroup IMX219_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup IMX219_Exported_Constants IMX219 Exported Constants
  * @{
  */
#define IMX219_REG_MODE_SELECT    0x0100
#define IMX219_MODE_STREAMING       0x01
#define IMX219_MODE_STANDBY         0x00

#define IMX219_REG_GAIN           0x0157
#define IMX219_REG_EXPOSURE       0x015A
#define IMX219_REG_FRAME_LENGTH   0x0160
#define IMX219_REG_LINE_LENGTH    0x0162
#define IMX219_REG_MIRROR_FLIP    0x0172
#define IMX219_REG_TEST_PATTERN   0x0600

#define IMX219_REG_ID             0x0000
#define IMX219_CHIP_ID            0x0219

#define IMX219_EXPOSURE_MIN       4
#define IMX219_EXPOSURE_DEFAULT   1000

#define IMX219_NAME               "IMX219"
#define IMX219_BAYER_PATTERN      0 /* RGGB Bayer pattern */
#define IMX219_COLOR_DEPTH        10 /* 10-bit RAW */
#define IMX219_GAIN_MIN           (0 * 1000)
#define IMX219_GAIN_MAX           (232 * 300) /* 232 in 0.3dB steps = 69.6dB */
#define IMX219_GAIN_DEFAULT       (0 * 1000)
#define IMX219_GAIN_UNIT_MDB      300 /* 0.3dB = 300 mdB */
#define IMX219_EXPOSURE_MIN       1           /* in us */
#define IMX219_EXPOSURE_MAX       50000       /* in us */

/* Resolution specific constants */
/* For 3280x2464 */
#define IMX219_WIDTH_3280         3280
#define IMX219_HEIGHT_2464        2464
#define IMX219_PCLK_3280_2464     280000000

/* For 1920x1080 */
#define IMX219_WIDTH_1920         1920
#define IMX219_HEIGHT_1080        1080
#define IMX219_PCLK_1920_1080     280000000

/* For 1640x1232 */
#define IMX219_WIDTH_1640         1640
#define IMX219_HEIGHT_1232        1232
#define IMX219_PCLK_1640_1232     280000000

/* For 640x480 */
#define IMX219_WIDTH_640          640
#define IMX219_HEIGHT_480         480
#define IMX219_PCLK_640_480       280000000

/**
  * @}
  */

/************** Generic Function  *******************/

typedef int32_t (*IMX219_Write_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*IMX219_Read_Func) (void *, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  IMX219_Write_Func   WriteReg;
  IMX219_Read_Func    ReadReg;
  void                *handle;
} imx219_ctx_t;

/*******************************************************************************
* Register      : Generic - All
* Address       : Generic - All
* Bit Group Name: None
* Permission    : W
*******************************************************************************/
int32_t imx219_write_reg(imx219_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t imx219_read_reg(imx219_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);

int32_t imx219_register_set(imx219_ctx_t *ctx, uint16_t reg, uint8_t value);


/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* IMX219_REG_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */