/**
  ******************************************************************************
  * @file    imx219_reg.h
  * @author  MCD Application Team
  * @brief   Header of imx219_reg.c
  *
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

/* Mode Control Registers */
#define IMX219_REG_MODE_SELECT      0x0100
#define IMX219_MODE_STANDBY           0x00
#define IMX219_MODE_STREAMING         0x01

/* Chip ID Register */
#define IMX219_REG_CHIP_ID          0x0000
#define IMX219_CHIP_ID              0x0219

/* Clock Configuration */
#define IMX219_XCLK_FREQ            24000000

/* Image Format Registers */
#define IMX219_REG_CSI_LANE_MODE    0x0114
#define IMX219_REG_DPHY_CTRL        0x0128
#define IMX219_REG_EXCK_FREQ        0x012A
#define IMX219_REG_EXCK_FREQ_H      0x012A
#define IMX219_REG_EXCK_FREQ_L      0x012B

/* Frame Timing Registers */
#define IMX219_REG_FRM_LENGTH_A     0x0160
#define IMX219_REG_FRM_LENGTH_A_H   0x0160
#define IMX219_REG_FRM_LENGTH_A_L   0x0161
#define IMX219_REG_LINE_LENGTH_A    0x0162
#define IMX219_REG_LINE_LENGTH_A_H  0x0162
#define IMX219_REG_LINE_LENGTH_A_L  0x0163

/* Crop Configuration */
#define IMX219_REG_X_ADD_STA_A      0x0164
#define IMX219_REG_X_ADD_STA_A_H    0x0164
#define IMX219_REG_X_ADD_STA_A_L    0x0165
#define IMX219_REG_X_ADD_END_A      0x0166
#define IMX219_REG_X_ADD_END_A_H    0x0166
#define IMX219_REG_X_ADD_END_A_L    0x0167
#define IMX219_REG_Y_ADD_STA_A      0x0168
#define IMX219_REG_Y_ADD_STA_A_H    0x0168
#define IMX219_REG_Y_ADD_STA_A_L    0x0169
#define IMX219_REG_Y_ADD_END_A      0x016A
#define IMX219_REG_Y_ADD_END_A_H    0x016A
#define IMX219_REG_Y_ADD_END_A_L    0x016B

/* Output Size Configuration */
#define IMX219_REG_X_OUTPUT_SIZE    0x016C
#define IMX219_REG_X_OUTPUT_SIZE_H  0x016C
#define IMX219_REG_X_OUTPUT_SIZE_L  0x016D
#define IMX219_REG_Y_OUTPUT_SIZE    0x016E
#define IMX219_REG_Y_OUTPUT_SIZE_H  0x016E
#define IMX219_REG_Y_OUTPUT_SIZE_L  0x016F

/* Binning Configuration */
#define IMX219_REG_X_ODD_INC_A      0x0170
#define IMX219_REG_Y_ODD_INC_A      0x0171
#define IMX219_REG_BINNING_MODE     0x0174
#define IMX219_REG_BINNING_MODE_H   0x0174
#define IMX219_REG_BINNING_MODE_L   0x0175

/* Exposure Control */
#define IMX219_REG_COARSE_INTEG_TIME 0x015A
#define IMX219_REG_COARSE_INTEG_TIME_H 0x015A
#define IMX219_REG_COARSE_INTEG_TIME_L 0x015B

/* Gain Control */
#define IMX219_REG_ANALOGUE_GAIN    0x0157
#define IMX219_REG_DIGITAL_GAIN     0x0158
#define IMX219_REG_DIGITAL_GAIN_H   0x0158
#define IMX219_REG_DIGITAL_GAIN_L   0x0159

/* Test Pattern */
#define IMX219_REG_TEST_PATTERN     0x0600
#define IMX219_REG_TEST_PATTERN_H   0x0600
#define IMX219_REG_TEST_PATTERN_L   0x0601

/* Mirror/Flip */
#define IMX219_REG_ORIENTATION      0x0172

/* Sensor Characteristics */
#define IMX219_NAME                 "IMX219"
#define IMX219_BAYER_PATTERN        0 /* RGGB */
#define IMX219_COLOR_DEPTH_8        8 /* 8 bits */
#define IMX219_COLOR_DEPTH_10       10 /* 10 bits */

/* Timing constraints */
#define IMX219_PIXEL_RATE           182400000
#define IMX219_DEFAULT_LINK_FREQ    456000000

/* Exposure Control */
#define IMX219_EXPOSURE_MIN         4
#define IMX219_EXPOSURE_MAX         65535
#define IMX219_EXPOSURE_DEFAULT     1000

/* Gain Control */
#define IMX219_ANALOGUE_GAIN_MIN    (0 * 1000)
#define IMX219_ANALOGUE_GAIN_MAX    (232 * 1000)
#define IMX219_ANALOGUE_GAIN_DEFAULT (20 * 1000)

#define IMX219_DIGITAL_GAIN_MIN     (1 * 1000)
#define IMX219_DIGITAL_GAIN_MAX     (4 * 1000)
#define IMX219_DIGITAL_GAIN_DEFAULT (1 * 1000)

/* Test Pattern Values */
#define IMX219_TEST_PATTERN_DISABLE    0
#define IMX219_TEST_PATTERN_SOLID      1
#define IMX219_TEST_PATTERN_COLOR_BARS 2
#define IMX219_TEST_PATTERN_GREY_BARS  3
#define IMX219_TEST_PATTERN_PN9        4

/* Mirror/Flip Values */
#define IMX219_FLIP_VERTICAL          0x01
#define IMX219_FLIP_HORIZONTAL        0x02

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