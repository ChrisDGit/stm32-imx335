/**
  ******************************************************************************
  * @file    imx219.c
  * @author  MCD Application Team
  * @brief   This file provides the IMX219 camera driver
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

/* Includes ------------------------------------------------------------------*/
#include "imx219.h"
#include <string.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup IMX219
  * @brief     This file provides a set of functions needed to drive the
  *            IMX219 Camera module.
  * @{
  */

/** @defgroup IMX219_Private_TypesDefinitions Private Types definition
  * @{
  */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
  * @}
  */

/** @defgroup IMX219_Private_Variables Private Variables
  * @{
  */
IMX219_CAMERA_Drv_t   IMX219_CAMERA_Driver =
{
  .Init = IMX219_Init,
  .DeInit = IMX219_DeInit,
  .ReadID = IMX219_ReadID,
  .GetCapabilities = IMX219_GetCapabilities,
  .SetGain = IMX219_SetGain,
  .SetExposure = IMX219_SetExposure,
  .SetFrequency = IMX219_SetFrequency,
  .MirrorFlipConfig = IMX219_MirrorFlipConfig,
  .GetSensorInfo = IMX219_GetSensorInfo,
  .SetTestPattern = IMX219_SetTestPattern
};
/**
  * @}
  */

/** @defgroup IMX219_Private_Constants Private Constants
  * @{
  */
struct regval {
  uint16_t addr;
  uint8_t val;
};

/* Common sensor setup */
static const struct regval imx219_common_regs[] = {
  {IMX219_REG_CSI_LANE_MODE, 0x01},      /* 2-lane CSI */
  {IMX219_REG_DPHY_CTRL, 0x00},          /* DPHY control */
  {IMX219_REG_EXCK_FREQ_H, 0x01},        /* External clock frequency */
  {IMX219_REG_EXCK_FREQ_L, 0x77},        /* 24MHz = 0x0177 */
  {0x0157, 0x00},                        /* Analogue gain */
  {0x0158, 0x01},                        /* Digital gain H */
  {0x0159, 0x00},                        /* Digital gain L */
  {0x015A, 0x00},                        /* Coarse integration time H */
  {0x015B, 0x00},                        /* Coarse integration time L */
  {0x0160, 0x0F},                        /* Frame length H */
  {0x0161, 0x2F},                        /* Frame length L */
  {0x0162, 0x0D},                        /* Line length H */
  {0x0163, 0x78},                        /* Line length L */
  {0x0164, 0x00},                        /* X_ADD_STA_A H */
  {0x0165, 0x00},                        /* X_ADD_STA_A L */
  {0x0166, 0x0C},                        /* X_ADD_END_A H */
  {0x0167, 0xCF},                        /* X_ADD_END_A L */
  {0x0168, 0x00},                        /* Y_ADD_STA_A H */
  {0x0169, 0x00},                        /* Y_ADD_STA_A L */
  {0x016A, 0x09},                        /* Y_ADD_END_A H */
  {0x016B, 0x9F},                        /* Y_ADD_END_A L */
  {0x016C, 0x0C},                        /* X_OUTPUT_SIZE H */
  {0x016D, 0xD0},                        /* X_OUTPUT_SIZE L */
  {0x016E, 0x09},                        /* Y_OUTPUT_SIZE H */
  {0x016F, 0xA0},                        /* Y_OUTPUT_SIZE L */
  {0x0170, 0x01},                        /* X_ODD_INC_A */
  {0x0171, 0x01},                        /* Y_ODD_INC_A */
  {0x0172, 0x00},                        /* IMG_ORIENTATION */
  {0x0174, 0x00},                        /* BINNING_MODE H */
  {0x0175, 0x00},                        /* BINNING_MODE L */
  {0x018C, 0x0A},                        /* CSI_DATA_FORMAT H */
  {0x018D, 0x0A},                        /* CSI_DATA_FORMAT L */
  {0x0301, 0x05},                        /* VTPXCK_DIV */
  {0x0303, 0x01},                        /* VTSYCK_DIV */
  {0x0304, 0x03},                        /* PREPLLCK_VT_DIV */
  {0x0305, 0x03},                        /* PREPLLCK_OP_DIV */
  {0x0306, 0x00},                        /* PLL_VT_MPY H */
  {0x0307, 0x39},                        /* PLL_VT_MPY L */
  {0x0309, 0x0A},                        /* OPPXCK_DIV */
  {0x030B, 0x01},                        /* OPSYCK_DIV */
  {0x030C, 0x00},                        /* PLL_OP_MPY H */
  {0x030D, 0x72},                        /* PLL_OP_MPY L */
  {0x455E, 0x00},                        /* CIS Tuning */
  {0x471E, 0x4B},                        /* CIS Tuning */
  {0x4767, 0x0F},                        /* CIS Tuning */
  {0x4750, 0x14},                        /* CIS Tuning */
  {0x4540, 0x00},                        /* CIS Tuning */
  {0x47B4, 0x14},                        /* CIS Tuning */
  {0x4713, 0x30},                        /* CIS Tuning */
  {0x478B, 0x10},                        /* CIS Tuning */
  {0x478F, 0x10},                        /* CIS Tuning */
  {0x4793, 0x10},                        /* CIS Tuning */
  {0x4797, 0x0E},                        /* CIS Tuning */
  {0x479B, 0x0E},                        /* CIS Tuning */
};

/* 3280x2464 8MP mode (15fps) */
static const struct regval imx219_3280x2464_regs[] = {
  {IMX219_REG_X_ADD_STA_A_H, 0x00},
  {IMX219_REG_X_ADD_STA_A_L, 0x00},
  {IMX219_REG_X_ADD_END_A_H, 0x0C},
  {IMX219_REG_X_ADD_END_A_L, 0xCF},
  {IMX219_REG_Y_ADD_STA_A_H, 0x00},
  {IMX219_REG_Y_ADD_STA_A_L, 0x00},
  {IMX219_REG_Y_ADD_END_A_H, 0x09},
  {IMX219_REG_Y_ADD_END_A_L, 0x9F},
  {IMX219_REG_X_OUTPUT_SIZE_H, 0x0C},
  {IMX219_REG_X_OUTPUT_SIZE_L, 0xD0},
  {IMX219_REG_Y_OUTPUT_SIZE_H, 0x09},
  {IMX219_REG_Y_OUTPUT_SIZE_L, 0xA0},
  {IMX219_REG_X_ODD_INC_A, 0x01},
  {IMX219_REG_Y_ODD_INC_A, 0x01},
  {IMX219_REG_BINNING_MODE_H, 0x00},
  {IMX219_REG_BINNING_MODE_L, 0x00},
  {IMX219_REG_FRM_LENGTH_A_H, 0x0A},
  {IMX219_REG_FRM_LENGTH_A_L, 0x3C},
  {IMX219_REG_LINE_LENGTH_A_H, 0x0D},
  {IMX219_REG_LINE_LENGTH_A_L, 0x78},
};

/* 1920x1080 1080p mode (30fps) */
static const struct regval imx219_1920x1080_regs[] = {
  {IMX219_REG_X_ADD_STA_A_H, 0x02},
  {IMX219_REG_X_ADD_STA_A_L, 0xA8},
  {IMX219_REG_X_ADD_END_A_H, 0x0A},
  {IMX219_REG_X_ADD_END_A_L, 0x27},
  {IMX219_REG_Y_ADD_STA_A_H, 0x02},
  {IMX219_REG_Y_ADD_STA_A_L, 0xB4},
  {IMX219_REG_Y_ADD_END_A_H, 0x06},
  {IMX219_REG_Y_ADD_END_A_L, 0xEB},
  {IMX219_REG_X_OUTPUT_SIZE_H, 0x07},
  {IMX219_REG_X_OUTPUT_SIZE_L, 0x80},
  {IMX219_REG_Y_OUTPUT_SIZE_H, 0x04},
  {IMX219_REG_Y_OUTPUT_SIZE_L, 0x38},
  {IMX219_REG_X_ODD_INC_A, 0x01},
  {IMX219_REG_Y_ODD_INC_A, 0x01},
  {IMX219_REG_BINNING_MODE_H, 0x00},
  {IMX219_REG_BINNING_MODE_L, 0x00},
  {IMX219_REG_FRM_LENGTH_A_H, 0x04},
  {IMX219_REG_FRM_LENGTH_A_L, 0x60},
  {IMX219_REG_LINE_LENGTH_A_H, 0x0D},
  {IMX219_REG_LINE_LENGTH_A_L, 0x78},
};

/* 1640x1232 2x2 binned mode (30fps) */
static const struct regval imx219_1640x1232_regs[] = {
  {IMX219_REG_X_ADD_STA_A_H, 0x00},
  {IMX219_REG_X_ADD_STA_A_L, 0x00},
  {IMX219_REG_X_ADD_END_A_H, 0x0C},
  {IMX219_REG_X_ADD_END_A_L, 0xCF},
  {IMX219_REG_Y_ADD_STA_A_H, 0x00},
  {IMX219_REG_Y_ADD_STA_A_L, 0x00},
  {IMX219_REG_Y_ADD_END_A_H, 0x09},
  {IMX219_REG_Y_ADD_END_A_L, 0x9F},
  {IMX219_REG_X_OUTPUT_SIZE_H, 0x06},
  {IMX219_REG_X_OUTPUT_SIZE_L, 0x68},
  {IMX219_REG_Y_OUTPUT_SIZE_H, 0x04},
  {IMX219_REG_Y_OUTPUT_SIZE_L, 0xD0},
  {IMX219_REG_X_ODD_INC_A, 0x03},
  {IMX219_REG_Y_ODD_INC_A, 0x03},
  {IMX219_REG_BINNING_MODE_H, 0x01},
  {IMX219_REG_BINNING_MODE_L, 0x01},
  {IMX219_REG_FRM_LENGTH_A_H, 0x05},
  {IMX219_REG_FRM_LENGTH_A_L, 0x1A},
  {IMX219_REG_LINE_LENGTH_A_H, 0x06},
  {IMX219_REG_LINE_LENGTH_A_L, 0xBC},
};

/* 640x480 VGA mode (30fps) */
static const struct regval imx219_640x480_regs[] = {
  {IMX219_REG_X_ADD_STA_A_H, 0x01},
  {IMX219_REG_X_ADD_STA_A_L, 0x58},
  {IMX219_REG_X_ADD_END_A_H, 0x0B},
  {IMX219_REG_X_ADD_END_A_L, 0x77},
  {IMX219_REG_Y_ADD_STA_A_H, 0x02},
  {IMX219_REG_Y_ADD_STA_A_L, 0x00},
  {IMX219_REG_Y_ADD_END_A_H, 0x07},
  {IMX219_REG_Y_ADD_END_A_L, 0x9F},
  {IMX219_REG_X_OUTPUT_SIZE_H, 0x02},
  {IMX219_REG_X_OUTPUT_SIZE_L, 0x80},
  {IMX219_REG_Y_OUTPUT_SIZE_H, 0x01},
  {IMX219_REG_Y_OUTPUT_SIZE_L, 0xE0},
  {IMX219_REG_X_ODD_INC_A, 0x07},
  {IMX219_REG_Y_ODD_INC_A, 0x07},
  {IMX219_REG_BINNING_MODE_H, 0x01},
  {IMX219_REG_BINNING_MODE_L, 0x01},
  {IMX219_REG_FRM_LENGTH_A_H, 0x02},
  {IMX219_REG_FRM_LENGTH_A_L, 0x0D},
  {IMX219_REG_LINE_LENGTH_A_H, 0x06},
  {IMX219_REG_LINE_LENGTH_A_L, 0xBC},
};

/* RAW8 pixel format configuration */
static const struct regval imx219_raw8_regs[] = {
  {0x018C, 0x08},                        /* CSI_DATA_FORMAT H */
  {0x018D, 0x08},                        /* CSI_DATA_FORMAT L */
};

/* RAW10 pixel format configuration */
static const struct regval imx219_raw10_regs[] = {
  {0x018C, 0x0A},                        /* CSI_DATA_FORMAT H */
  {0x018D, 0x0A},                        /* CSI_DATA_FORMAT L */
};

/* Test pattern configurations */
static const struct regval test_pattern_solid_regs[] = {
  {IMX219_REG_TEST_PATTERN_H, 0x00},
  {IMX219_REG_TEST_PATTERN_L, 0x01},
};

static const struct regval test_pattern_color_bars_regs[] = {
  {IMX219_REG_TEST_PATTERN_H, 0x00},
  {IMX219_REG_TEST_PATTERN_L, 0x02},
};

static const struct regval test_pattern_grey_bars_regs[] = {
  {IMX219_REG_TEST_PATTERN_H, 0x00},
  {IMX219_REG_TEST_PATTERN_L, 0x03},
};

static const struct regval test_pattern_pn9_regs[] = {
  {IMX219_REG_TEST_PATTERN_H, 0x00},
  {IMX219_REG_TEST_PATTERN_L, 0x04},
};

static const struct regval test_pattern_disabled_regs[] = {
  {IMX219_REG_TEST_PATTERN_H, 0x00},
  {IMX219_REG_TEST_PATTERN_L, 0x00},
};

/* Mirror/flip configurations */
static const struct regval mirrorflip_mode_regs[][2] = {
  {
    {IMX219_REG_ORIENTATION, 0x00},      /* Normal */
  },
  {
    {IMX219_REG_ORIENTATION, 0x01},      /* Vertical flip */
  },
  {
    {IMX219_REG_ORIENTATION, 0x02},      /* Horizontal flip */
  },
  {
    {IMX219_REG_ORIENTATION, 0x03},      /* Both flip */
  },
};

/**
  * @}
  */

/** @defgroup IMX219_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t IMX219_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t IMX219_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t IMX219_WriteTable(IMX219_Object_t *pObj, const struct regval *regs, uint32_t num_regs);
/**
  * @}
  */

/** @defgroup IMX219_Private_Functions Private Functions
  * @{
  */

/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  handle  Component object handle
  * @param  Reg     The target register address to read
  * @param  pData   The target register value to be read
  * @param  Length  buffer size
  * @retval error status
  */
static int32_t IMX219_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  IMX219_Object_t *pObj = (IMX219_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg     The target register address to write
  * @param  pData   The target register value to be written
  * @param  Length  buffer size
  * @retval error status
  */
static int32_t IMX219_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  IMX219_Object_t *pObj = (IMX219_Object_t *)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Write an array of register values to the device
  * @param  pObj    Component object handle
  * @param  regs    Array of register values
  * @param  num_regs Number of registers to write
  * @retval error status
  */
static int32_t IMX219_WriteTable(IMX219_Object_t *pObj, const struct regval *regs, uint32_t num_regs)
{
  int32_t ret = IMX219_OK;
  uint32_t i;

  for (i = 0; i < num_regs; i++)
  {
    if (imx219_register_set(&pObj->Ctx, regs[i].addr, regs[i].val) != IMX219_OK)
    {
      ret = IMX219_ERROR;
      break;
    }
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup IMX219_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Register Bus IO operations to component object
  * @param  pObj Component object pointer
  * @param  pIO  IO operations structure
  * @retval error status
  */
int32_t IMX219_RegisterBusIO(IMX219_Object_t *pObj, IMX219_IO_t *pIO)
{
  int32_t ret = IMX219_OK;

  if (pObj == NULL)
  {
    ret = IMX219_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.ReadReg  = IMX219_ReadRegWrap;
    pObj->Ctx.WriteReg = IMX219_WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if(pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = IMX219_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initializes the IMX219 CAMERA component.
  * @param  pObj  pointer to component object
  * @param  Resolution  Camera resolution
  * @param  PixelFormat pixel format to be configured
  * @retval Component status
  */
int32_t IMX219_Init(IMX219_Object_t *pObj, uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret = IMX219_OK;
  uint8_t tmp;

  if(pObj->IsInitialized == 0U)
  {
    /* Write common configuration */
    if(IMX219_WriteTable(pObj, imx219_common_regs, ARRAY_SIZE(imx219_common_regs)) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }

    if(ret == IMX219_OK)
    {
      /* Configure resolution */
      switch (Resolution)
      {
        case IMX219_R3280_2464:
          if(IMX219_WriteTable(pObj, imx219_3280x2464_regs, ARRAY_SIZE(imx219_3280x2464_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        case IMX219_R1920_1080:
          if(IMX219_WriteTable(pObj, imx219_1920x1080_regs, ARRAY_SIZE(imx219_1920x1080_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        case IMX219_R1640_1232:
          if(IMX219_WriteTable(pObj, imx219_1640x1232_regs, ARRAY_SIZE(imx219_1640x1232_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        case IMX219_R640_480:
          if(IMX219_WriteTable(pObj, imx219_640x480_regs, ARRAY_SIZE(imx219_640x480_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        default:
          /* Resolution not supported */
          ret = IMX219_ERROR;
          break;
      }
    }

    if(ret == IMX219_OK)
    {
      /* Configure pixel format */
      switch (PixelFormat)
      {
        case IMX219_RAW_RGGB8:
          if(IMX219_WriteTable(pObj, imx219_raw8_regs, ARRAY_SIZE(imx219_raw8_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        case IMX219_RAW_RGGB10:
          if(IMX219_WriteTable(pObj, imx219_raw10_regs, ARRAY_SIZE(imx219_raw10_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        default:
          /* Pixel format not supported */
          ret = IMX219_ERROR;
          break;
      }
    }

    if(ret == IMX219_OK)
    {
      /* Start streaming */
      tmp = IMX219_MODE_STREAMING;
      if (imx219_write_reg(&pObj->Ctx, IMX219_REG_MODE_SELECT, &tmp, 1) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      else
      {
        pObj->IsInitialized = 1U;
      }
    }
  }

  return ret;
}

/**
  * @brief  Deinitializes the IMX219 CAMERA component.
  * @param  pObj  pointer to component object
  * @retval Component status
  */
int32_t IMX219_DeInit(IMX219_Object_t *pObj)
{
  int32_t ret = IMX219_OK;
  uint8_t tmp;

  if(pObj->IsInitialized == 1U)
  {
    /* Stop streaming */
    tmp = IMX219_MODE_STANDBY;
    if (imx219_write_reg(&pObj->Ctx, IMX219_REG_MODE_SELECT, &tmp, 1) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }
    else
    {
      pObj->IsInitialized = 0U;
    }
  }

  return ret;
}

/**
  * @brief  Reads the IMX219 CAMERA component ID.
  * @param  pObj  pointer to component object
  * @param  Id    pointer to component ID
  * @retval Component status
  */
int32_t IMX219_ReadID(IMX219_Object_t *pObj, uint32_t *Id)
{
  int32_t ret = IMX219_OK;
  uint8_t tmp[2];

  if (imx219_read_reg(&pObj->Ctx, IMX219_REG_CHIP_ID, tmp, 2) != IMX219_OK)
  {
    ret = IMX219_ERROR;
  }
  else
  {
    *Id = ((uint32_t)tmp[0] << 8) | tmp[1];
  }

  return ret;
}

/**
  * @brief  Get IMX219 component capabilities
  * @param  pObj         pointer to component object
  * @param  Capabilities pointer to component capabilities
  * @retval Component status
  */
int32_t IMX219_GetCapabilities(IMX219_Object_t *pObj, IMX219_Capabilities_t *Capabilities)
{
  int32_t ret = IMX219_OK;

  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Set supported capabilities */
  Capabilities->Config_Resolution = 1U;
  Capabilities->Config_LightMode = 0U;
  Capabilities->Config_SpecialEffect = 0U;
  Capabilities->Config_Brightness = 0U;
  Capabilities->Config_Saturation = 0U;
  Capabilities->Config_Contrast = 0U;
  Capabilities->Config_HueDegree = 0U;
  Capabilities->Config_Gain = 1U;
  Capabilities->Config_Exposure = 1U;
  Capabilities->Config_MirrorFlip = 1U;
  Capabilities->Config_Zoom = 0U;
  Capabilities->Config_NightMode = 0U;
  Capabilities->Config_ExposureMode = 0U;
  Capabilities->Config_SensorInfo = 1U;
  Capabilities->Config_TestPattern = 1U;
  Capabilities->Config_ColorEffect = 0U;

  return ret;
}

/**
  * @brief  Set IMX219 gain value
  * @param  pObj  pointer to component object
  * @param  gain  gain value to be set
  * @retval Component status
  */
int32_t IMX219_SetGain(IMX219_Object_t *pObj, int32_t gain)
{
  int32_t ret = IMX219_OK;
  uint8_t analogue_gain;

  /* Convert gain to analogue gain register value */
  /* The gain is in milli-units (e.g., 20000 = 20.0x gain) */
  if (gain < IMX219_ANALOGUE_GAIN_MIN)
  {
    gain = IMX219_ANALOGUE_GAIN_MIN;
  }
  else if (gain > IMX219_ANALOGUE_GAIN_MAX)
  {
    gain = IMX219_ANALOGUE_GAIN_MAX;
  }

  /* Calculate analogue gain register value */
  analogue_gain = (uint8_t)(256 - (256 * 1000) / gain);

  if (imx219_register_set(&pObj->Ctx, IMX219_REG_ANALOGUE_GAIN, analogue_gain) != IMX219_OK)
  {
    ret = IMX219_ERROR;
  }

  return ret;
}

/**
  * @brief  Set IMX219 exposure value
  * @param  pObj     pointer to component object
  * @param  exposure exposure value to be set
  * @retval Component status
  */
int32_t IMX219_SetExposure(IMX219_Object_t *pObj, int32_t exposure)
{
  int32_t ret = IMX219_OK;
  uint8_t exposure_h, exposure_l;

  /* Clamp exposure value */
  if (exposure < IMX219_EXPOSURE_MIN)
  {
    exposure = IMX219_EXPOSURE_MIN;
  }
  else if (exposure > IMX219_EXPOSURE_MAX)
  {
    exposure = IMX219_EXPOSURE_MAX;
  }

  /* Split exposure value into high and low bytes */
  exposure_h = (uint8_t)((exposure >> 8) & 0xFF);
  exposure_l = (uint8_t)(exposure & 0xFF);

  if (imx219_register_set(&pObj->Ctx, IMX219_REG_COARSE_INTEG_TIME_H, exposure_h) != IMX219_OK)
  {
    ret = IMX219_ERROR;
  }
  else if (imx219_register_set(&pObj->Ctx, IMX219_REG_COARSE_INTEG_TIME_L, exposure_l) != IMX219_OK)
  {
    ret = IMX219_ERROR;
  }

  return ret;
}

/**
  * @brief  Set IMX219 frequency
  * @param  pObj      pointer to component object
  * @param  frequency frequency value to be set
  * @retval Component status
  */
int32_t IMX219_SetFrequency(IMX219_Object_t *pObj, int32_t frequency)
{
  int32_t ret = IMX219_OK;

  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);
  (void)(frequency);

  /* IMX219 only supports 24MHz external clock */
  /* Frequency is already configured in common registers */

  return ret;
}

/**
  * @brief  Set IMX219 frame rate
  * @param  pObj      pointer to component object
  * @param  framerate frame rate value to be set
  * @retval Component status
  */
int32_t IMX219_SetFramerate(IMX219_Object_t *pObj, int32_t framerate)
{
  int32_t ret = IMX219_OK;

  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);
  (void)(framerate);

  /* Frame rate is configured by resolution mode */
  /* No dynamic frame rate adjustment implemented */

  return ret;
}

/**
  * @brief  Set IMX219 mirror flip configuration
  * @param  pObj   pointer to component object
  * @param  Config mirror flip configuration
  * @retval Component status
  */
int32_t IMX219_MirrorFlipConfig(IMX219_Object_t *pObj, uint32_t Config)
{
  int32_t ret = IMX219_OK;

  if (Config <= IMX219_MIRROR_FLIP)
  {
    if (IMX219_WriteTable(pObj, mirrorflip_mode_regs[Config], 1) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }
  }
  else
  {
    ret = IMX219_ERROR;
  }

  return ret;
}

/**
  * @brief  Get IMX219 sensor information
  * @param  pObj  pointer to component object
  * @param  Info  pointer to sensor information structure
  * @retval Component status
  */
int32_t IMX219_GetSensorInfo(IMX219_Object_t *pObj, IMX219_SensorInfo_t *Info)
{
  int32_t ret = IMX219_OK;

  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Fill sensor information */
  (void)strncpy(Info->name, IMX219_NAME, sizeof(Info->name) - 1);
  Info->name[sizeof(Info->name) - 1] = '\0';
  Info->bayer_pattern = IMX219_BAYER_PATTERN;
  Info->color_depth = IMX219_COLOR_DEPTH_10;
  Info->width = 3280;
  Info->height = 2464;
  Info->gain_min = IMX219_ANALOGUE_GAIN_MIN;
  Info->gain_max = IMX219_ANALOGUE_GAIN_MAX;
  Info->exposure_min = IMX219_EXPOSURE_MIN;
  Info->exposure_max = IMX219_EXPOSURE_MAX;

  return ret;
}

/**
  * @brief  Set IMX219 test pattern
  * @param  pObj  pointer to component object
  * @param  mode  test pattern mode
  * @retval Component status
  */
int32_t IMX219_SetTestPattern(IMX219_Object_t *pObj, int32_t mode)
{
  int32_t ret = IMX219_OK;

  switch (mode)
  {
    case IMX219_TEST_PATTERN_DISABLED:
      if (IMX219_WriteTable(pObj, test_pattern_disabled_regs, ARRAY_SIZE(test_pattern_disabled_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    case IMX219_TEST_PATTERN_SOLID:
      if (IMX219_WriteTable(pObj, test_pattern_solid_regs, ARRAY_SIZE(test_pattern_solid_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    case IMX219_TEST_PATTERN_COLOR_BARS:
      if (IMX219_WriteTable(pObj, test_pattern_color_bars_regs, ARRAY_SIZE(test_pattern_color_bars_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    case IMX219_TEST_PATTERN_GREY_BARS:
      if (IMX219_WriteTable(pObj, test_pattern_grey_bars_regs, ARRAY_SIZE(test_pattern_grey_bars_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    case IMX219_TEST_PATTERN_PN9:
      if (IMX219_WriteTable(pObj, test_pattern_pn9_regs, ARRAY_SIZE(test_pattern_pn9_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    default:
      ret = IMX219_ERROR;
      break;
  }

  return ret;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */