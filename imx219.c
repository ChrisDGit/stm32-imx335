/**
  ******************************************************************************
  * @file    imx219.c
  * @author  MCD Application Team
  * @brief   This file provides the IMX219 camera driver
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
  .SetLightMode = NULL,
  .SetColorEffect = NULL,
  .SetBrightness = NULL,
  .SetSaturation = NULL,
  .SetContrast = NULL,
  .SetHueDegree = NULL,
  .MirrorFlipConfig = IMX219_MirrorFlipConfig,
  .ZoomConfig = NULL,
  .SetResolution = NULL,
  .GetResolution = NULL,
  .SetPixelFormat = NULL,
  .GetPixelFormat = NULL,
  .NightModeConfig = NULL,
  .SetFrequency = IMX219_SetFrequency,
  .SetGain = IMX219_SetGain,
  .SetExposure = IMX219_SetExposure,
  .SetExposureMode = NULL,
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

/* 3280x2464 Resolution Register Configuration */
static const struct regval res_3280_2464_regs[] = {
  {0x0100, 0x00}, /* mode_select */
  {0x30EB, 0x05}, /* access_code */
  {0x30EB, 0x0C},
  {0x300A, 0xFF},
  {0x300B, 0xFF},
  {0x30EB, 0x05},
  {0x30EB, 0x09},
  {0x0114, 0x01}, /* orientation */
  {0x0128, 0x00}, /* dout_en */
  {0x012A, 0x18}, /* inck_freq */
  {0x012B, 0x00},
  {0x015A, 0x01}, /* coarse_integration_time */
  {0x015B, 0xF4},
  {0x0160, 0x0A}, /* frame_length_lines */
  {0x0161, 0x0D},
  {0x0162, 0x0D}, /* line_length_pck */
  {0x0163, 0x78},
  {0x0164, 0x00}, /* x_addr_start */
  {0x0165, 0x00},
  {0x0166, 0x0C}, /* x_addr_end */
  {0x0167, 0xCF},
  {0x0168, 0x00}, /* y_addr_start */
  {0x0169, 0x00},
  {0x016A, 0x09}, /* y_addr_end */
  {0x016B, 0x9F},
  {0x016C, 0x0C}, /* x_output_size */
  {0x016D, 0xD0},
  {0x016E, 0x09}, /* y_output_size */
  {0x016F, 0xA0},
  {0x0170, 0x01}, /* x_odd_inc */
  {0x0171, 0x01}, /* y_odd_inc */
  {0x0172, 0x00}, /* image_orientation */
  {0x0174, 0x00}, /* binning_mode */
  {0x0175, 0x00},
  {0x018C, 0x0A},
  {0x018D, 0x0A},
  {0x0301, 0x05}, /* vtpxck_div */
  {0x0303, 0x01}, /* vtsyck_div */
  {0x0304, 0x03}, /* prepllck_vt_div */
  {0x0305, 0x03},
  {0x0306, 0x00}, /* pll_vt_mpy */
  {0x0307, 0x39},
  {0x0309, 0x0A}, /* oppxck_div */
  {0x030B, 0x01}, /* opsyck_div */
  {0x030C, 0x00}, /* pll_op_mpy */
  {0x030D, 0x72}
};

/* 1920x1080 Resolution Register Configuration */
static const struct regval res_1920_1080_regs[] = {
  {0x0100, 0x00}, /* mode_select */
  {0x30EB, 0x05}, /* access_code */
  {0x30EB, 0x0C},
  {0x300A, 0xFF},
  {0x300B, 0xFF},
  {0x30EB, 0x05},
  {0x30EB, 0x09},
  {0x0114, 0x01}, /* orientation */
  {0x0128, 0x00}, /* dout_en */
  {0x012A, 0x18}, /* inck_freq */
  {0x012B, 0x00},
  {0x015A, 0x01}, /* coarse_integration_time */
  {0x015B, 0xF4},
  {0x0160, 0x06}, /* frame_length_lines */
  {0x0161, 0xE3},
  {0x0162, 0x0D}, /* line_length_pck */
  {0x0163, 0x78},
  {0x0164, 0x02}, /* x_addr_start */
  {0x0165, 0xA8},
  {0x0166, 0x0A}, /* x_addr_end */
  {0x0167, 0x27},
  {0x0168, 0x02}, /* y_addr_start */
  {0x0169, 0xB4},
  {0x016A, 0x06}, /* y_addr_end */
  {0x016B, 0xEB},
  {0x016C, 0x07}, /* x_output_size */
  {0x016D, 0x80},
  {0x016E, 0x04}, /* y_output_size */
  {0x016F, 0x38},
  {0x0170, 0x01}, /* x_odd_inc */
  {0x0171, 0x01}, /* y_odd_inc */
  {0x0172, 0x00}, /* image_orientation */
  {0x0174, 0x00}, /* binning_mode */
  {0x0175, 0x00},
  {0x018C, 0x0A},
  {0x018D, 0x0A},
  {0x0301, 0x05}, /* vtpxck_div */
  {0x0303, 0x01}, /* vtsyck_div */
  {0x0304, 0x03}, /* prepllck_vt_div */
  {0x0305, 0x03},
  {0x0306, 0x00}, /* pll_vt_mpy */
  {0x0307, 0x39},
  {0x0309, 0x0A}, /* oppxck_div */
  {0x030B, 0x01}, /* opsyck_div */
  {0x030C, 0x00}, /* pll_op_mpy */
  {0x030D, 0x72}
};

/* 1640x1232 Resolution Register Configuration (2x2 binned) */
static const struct regval res_1640_1232_regs[] = {
  {0x0100, 0x00}, /* mode_select */
  {0x30EB, 0x05}, /* access_code */
  {0x30EB, 0x0C},
  {0x300A, 0xFF},
  {0x300B, 0xFF},
  {0x30EB, 0x05},
  {0x30EB, 0x09},
  {0x0114, 0x01}, /* orientation */
  {0x0128, 0x00}, /* dout_en */
  {0x012A, 0x18}, /* inck_freq */
  {0x012B, 0x00},
  {0x015A, 0x01}, /* coarse_integration_time */
  {0x015B, 0xF4},
  {0x0160, 0x05}, /* frame_length_lines */
  {0x0161, 0x1A},
  {0x0162, 0x0D}, /* line_length_pck */
  {0x0163, 0x78},
  {0x0164, 0x00}, /* x_addr_start */
  {0x0165, 0x00},
  {0x0166, 0x0C}, /* x_addr_end */
  {0x0167, 0xCF},
  {0x0168, 0x00}, /* y_addr_start */
  {0x0169, 0x00},
  {0x016A, 0x09}, /* y_addr_end */
  {0x016B, 0x9F},
  {0x016C, 0x06}, /* x_output_size */
  {0x016D, 0x68},
  {0x016E, 0x04}, /* y_output_size */
  {0x016F, 0xD0},
  {0x0170, 0x03}, /* x_odd_inc */
  {0x0171, 0x03}, /* y_odd_inc */
  {0x0172, 0x00}, /* image_orientation */
  {0x0174, 0x01}, /* binning_mode */
  {0x0175, 0x01},
  {0x018C, 0x0A},
  {0x018D, 0x0A},
  {0x0301, 0x05}, /* vtpxck_div */
  {0x0303, 0x01}, /* vtsyck_div */
  {0x0304, 0x03}, /* prepllck_vt_div */
  {0x0305, 0x03},
  {0x0306, 0x00}, /* pll_vt_mpy */
  {0x0307, 0x39},
  {0x0309, 0x0A}, /* oppxck_div */
  {0x030B, 0x01}, /* opsyck_div */
  {0x030C, 0x00}, /* pll_op_mpy */
  {0x030D, 0x72}
};

/* 640x480 Resolution Register Configuration */
static const struct regval res_640_480_regs[] = {
  {0x0100, 0x00}, /* mode_select */
  {0x30EB, 0x05}, /* access_code */
  {0x30EB, 0x0C},
  {0x300A, 0xFF},
  {0x300B, 0xFF},
  {0x30EB, 0x05},
  {0x30EB, 0x09},
  {0x0114, 0x01}, /* orientation */
  {0x0128, 0x00}, /* dout_en */
  {0x012A, 0x18}, /* inck_freq */
  {0x012B, 0x00},
  {0x015A, 0x01}, /* coarse_integration_time */
  {0x015B, 0xF4},
  {0x0160, 0x02}, /* frame_length_lines */
  {0x0161, 0x08},
  {0x0162, 0x0D}, /* line_length_pck */
  {0x0163, 0x78},
  {0x0164, 0x03}, /* x_addr_start */
  {0x0165, 0xE8},
  {0x0166, 0x08}, /* x_addr_end */
  {0x0167, 0xE7},
  {0x0168, 0x02}, /* y_addr_start */
  {0x0169, 0xF0},
  {0x016A, 0x06}, /* y_addr_end */
  {0x016B, 0xAF},
  {0x016C, 0x02}, /* x_output_size */
  {0x016D, 0x80},
  {0x016E, 0x01}, /* y_output_size */
  {0x016F, 0xE0},
  {0x0170, 0x03}, /* x_odd_inc */
  {0x0171, 0x03}, /* y_odd_inc */
  {0x0172, 0x00}, /* image_orientation */
  {0x0174, 0x03}, /* binning_mode */
  {0x0175, 0x03},
  {0x018C, 0x0A},
  {0x018D, 0x0A},
  {0x0301, 0x05}, /* vtpxck_div */
  {0x0303, 0x01}, /* vtsyck_div */
  {0x0304, 0x03}, /* prepllck_vt_div */
  {0x0305, 0x03},
  {0x0306, 0x00}, /* pll_vt_mpy */
  {0x0307, 0x39},
  {0x0309, 0x0A}, /* oppxck_div */
  {0x030B, 0x01}, /* opsyck_div */
  {0x030C, 0x00}, /* pll_op_mpy */
  {0x030D, 0x72}
};

/* Frequency configuration registers */
static const struct regval inck_24Mhz_regs[] = {
  {0x012A, 0x18},
  {0x012B, 0x00}
};

static const struct regval inck_6Mhz_regs[] = {
  {0x012A, 0x06},
  {0x012B, 0x00}
};

static const struct regval inck_27Mhz_regs[] = {
  {0x012A, 0x1B},
  {0x012B, 0x00}
};

/* Mirror/Flip configuration */
static const struct regval mirrorflip_mode_regs[][1] = {
  {
    {0x0172, 0x00}, /* Normal */
  },
  {
    {0x0172, 0x01}, /* Vertical flip */
  },
  {
    {0x0172, 0x02}, /* Horizontal mirror */
  },
  {
    {0x0172, 0x03}, /* Mirror and flip */
  },
};

/* Test Pattern Enable */
static const struct regval test_pattern_enable_regs[] = {
  {0x0600, 0x00},
  {0x0601, 0x02}
};

/* Test Pattern Disable */
static const struct regval test_pattern_disable_regs[] = {
  {0x0600, 0x00},
  {0x0601, 0x00}
};

#define IMX219_1H_PERIOD_USEC (1000000.0F / 3448 / 30) /* Approximate line time */

/**
  * @}
  */

/** @defgroup IMX219_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t IMX219_WriteTable(IMX219_Object_t *pObj, const struct regval *regs, uint32_t size);
static int32_t IMX219_ReadRegWrap(void *handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t IMX219_WriteRegWrap(void *handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t IMX219_Delay(IMX219_Object_t *pObj, uint32_t Delay);

/**
  * @}
  */

/** @defgroup IMX219_Private_Functions Private Functions
  * @{
  */
static int32_t IMX219_WriteTable(IMX219_Object_t *pObj, const struct regval *regs, uint32_t size)
{
  uint32_t index;
  int32_t ret = IMX219_OK;

  /* Set registers */
  for(index=0; index<size ; index++)
  {
    if(ret != IMX219_ERROR)
    {
      if(imx219_write_reg(&pObj->Ctx, regs[index].addr, (uint8_t *)&(regs[index].val), 1) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
    }
  }
  return ret;
}

/**
  * @brief This function provides accurate delay (in milliseconds)
  * @param pObj   pointer to component object
  * @param Delay  specifies the delay time length, in milliseconds
  * @retval IMX219_OK
  */
static int32_t IMX219_Delay(IMX219_Object_t *pObj, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while((pObj->IO.GetTick() - tickstart) < Delay)
  {
  }
  return IMX219_OK;
}

/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t IMX219_ReadRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  IMX219_Object_t *pObj = (IMX219_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t IMX219_WriteRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  IMX219_Object_t *pObj = (IMX219_Object_t *)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}
/**
  * @}
  */

/** @defgroup IMX219_Public_Functions Public Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj  pointer to component object
  * @param  pIO   pointer to IO functions structure
  * @retval Component status
  */
int32_t IMX219_RegisterBusIO(IMX219_Object_t *pObj, IMX219_IO_t *pIO)
{
  int32_t ret;

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
  * @brief  Initializes the camera sensor.
  * @param  pObj  pointer to component object
  * @param  Resolution  Camera resolution
  * @param  PixelFormat Pixel format
  * @retval Component status
  */
int32_t IMX219_Init(IMX219_Object_t *pObj, uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret = IMX219_OK;

  if(pObj->IsInitialized == 0U)
  {
    /* Power down */
    uint8_t tmp = IMX219_MODE_STANDBY;
    if(imx219_write_reg(&pObj->Ctx, IMX219_REG_MODE_SELECT, &tmp, 1) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }
    else
    {
      IMX219_Delay(pObj, 1);

      /* Set resolution configuration */
      switch (Resolution)
      {
        case IMX219_R3280_2464:
          if(IMX219_WriteTable(pObj, res_3280_2464_regs, ARRAY_SIZE(res_3280_2464_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        case IMX219_R1920_1080:
          if(IMX219_WriteTable(pObj, res_1920_1080_regs, ARRAY_SIZE(res_1920_1080_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        case IMX219_R1640_1232:
          if(IMX219_WriteTable(pObj, res_1640_1232_regs, ARRAY_SIZE(res_1640_1232_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        case IMX219_R640_480:
          if(IMX219_WriteTable(pObj, res_640_480_regs, ARRAY_SIZE(res_640_480_regs)) != IMX219_OK)
          {
            ret = IMX219_ERROR;
          }
          break;
        default:
          ret = IMX219_ERROR;
          break;
      }

      if(ret == IMX219_OK)
      {
        IMX219_Delay(pObj, 1);

        /* Power up */
        tmp = IMX219_MODE_STREAMING;
        if(imx219_write_reg(&pObj->Ctx, IMX219_REG_MODE_SELECT, &tmp, 1) != IMX219_OK)
        {
          ret = IMX219_ERROR;
        }
        else
        {
          IMX219_Delay(pObj, 1);
          pObj->IsInitialized = 1U;
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  De-initializes the camera sensor.
  * @param  pObj  pointer to component object
  * @retval Component status
  */
int32_t IMX219_DeInit(IMX219_Object_t *pObj)
{
  if(pObj->IsInitialized == 1U)
  {
    /* De-initialize camera sensor interface */
    pObj->IsInitialized = 0U;
  }

  return IMX219_OK;
}

/**
  * @brief  Read the IMX219 Camera identity.
  * @param  pObj  pointer to component object
  * @param  Id    pointer to component ID
  * @retval Component status
  */
int32_t IMX219_ReadID(IMX219_Object_t *pObj, uint32_t *Id)
{
  int32_t ret;
  uint8_t tmp[2];

  /* Initialize I2C */
  pObj->IO.Init();

  if(imx219_read_reg(&pObj->Ctx, IMX219_REG_ID, tmp, 2) != IMX219_OK)
  {
    ret = IMX219_ERROR;
  }
  else
  {
    *Id = ((uint32_t)tmp[0] << 8) | tmp[1];
    ret = IMX219_OK;
  }

  /* Component status */
  return ret;
}

/**
  * @brief  Read the IMX219 Camera Capabilities.
  * @param  pObj          pointer to component object
  * @param  Capabilities  pointer to component Capabilities
  * @retval Component status
  */
int32_t IMX219_GetCapabilities(IMX219_Object_t *pObj, IMX219_Capabilities_t *Capabilities)
{
  int32_t ret;

  if(pObj == NULL)
  {
    ret = IMX219_ERROR;
  }
  else
  {
    Capabilities->Config_Brightness    = 0;
    Capabilities->Config_Contrast      = 0;
    Capabilities->Config_HueDegree     = 0;
    Capabilities->Config_Gain          = 1;
    Capabilities->Config_Exposure      = 1;
    Capabilities->Config_ExposureMode  = 0;
    Capabilities->Config_LightMode     = 0;
    Capabilities->Config_MirrorFlip    = 1;
    Capabilities->Config_NightMode     = 0;
    Capabilities->Config_Resolution    = 1;
    Capabilities->Config_Saturation    = 0;
    Capabilities->Config_SpecialEffect = 0;
    Capabilities->Config_Zoom          = 0;
    Capabilities->Config_SensorInfo    = 1;
    Capabilities->Config_TestPattern   = 1;

    ret = IMX219_OK;
  }

  return ret;
}

/**
  * @brief  Get the IMX219 Sensor info.
  * @param  pObj   pointer to component object
  * @param  Info   pointer to sensor info
  * @retval Component status
  */
int32_t IMX219_GetSensorInfo(IMX219_Object_t *pObj, IMX219_SensorInfo_t *Info)
{
  if ((!pObj) || (Info == NULL))
  {
    return IMX219_ERROR;
  }

  if (sizeof(Info->name) >= strlen(IMX219_NAME) + 1)
  {
    strcpy(Info->name, IMX219_NAME);
  }
  else
  {
    return IMX219_ERROR;
  }

  Info->bayer_pattern = IMX219_BAYER_PATTERN;
  Info->color_depth = IMX219_COLOR_DEPTH;
  Info->width = IMX219_WIDTH_3280;
  Info->height = IMX219_HEIGHT_2464;
  Info->gain_min = IMX219_GAIN_MIN;
  Info->gain_max = IMX219_GAIN_MAX;
  Info->exposure_min = IMX219_EXPOSURE_MIN;
  Info->exposure_max = IMX219_EXPOSURE_MAX;

  return IMX219_OK;
}

/**
  * @brief  Set the gain
  * @param  pObj  pointer to component object
  * @param  Gain Gain in mdB (0.3dB = 300 mdB)
  * @retval Component status
  */
int32_t IMX219_SetGain(IMX219_Object_t *pObj, int32_t gain)
{
  int32_t ret = IMX219_OK;
  uint8_t gain_val;

  if ((gain > IMX219_GAIN_MAX) || (gain < IMX219_GAIN_MIN))
  {
    ret = IMX219_ERROR;
  }
  else
  {
    /* Convert to IMX219 gain unit (0.3 dB = 300 mdB) */
    gain_val = (uint8_t)(gain / IMX219_GAIN_UNIT_MDB);

    if(imx219_write_reg(&pObj->Ctx, IMX219_REG_GAIN, &gain_val, 1) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Set the exposure
  * @param  pObj  pointer to component object
  * @param  Exposure Exposure in micro seconds
  * @retval Component status
  */
int32_t IMX219_SetExposure(IMX219_Object_t *pObj, int32_t exposure)
{
  int32_t ret = IMX219_OK;
  uint32_t frame_length, coarse_time;
  uint8_t exposure_regs[2];

  if (imx219_read_reg(&pObj->Ctx, IMX219_REG_FRAME_LENGTH, (uint8_t *)&frame_length, 2) != IMX219_OK)
  {
    ret = IMX219_ERROR;
  }
  else
  {
    /* Convert exposure time to line count */
    coarse_time = exposure / ((uint32_t) IMX219_1H_PERIOD_USEC);

    /* Ensure exposure doesn't exceed frame length - 4 */
    if (coarse_time > (frame_length - 4))
    {
      coarse_time = frame_length - 4;
    }
    if (coarse_time < IMX219_EXPOSURE_MIN)
    {
      coarse_time = IMX219_EXPOSURE_MIN;
    }

    exposure_regs[0] = (uint8_t)((coarse_time >> 8) & 0xFF);
    exposure_regs[1] = (uint8_t)(coarse_time & 0xFF);

    if(imx219_write_reg(&pObj->Ctx, IMX219_REG_EXPOSURE, exposure_regs, 2) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Set the input clock frequency
  * @param  pObj  pointer to component object
  * @param  Frequency Input clock frequency
  * @retval Component status
  */
int32_t IMX219_SetFrequency(IMX219_Object_t *pObj, int32_t frequency)
{
  int32_t ret = IMX219_OK;

  switch (frequency)
  {
    case IMX219_INCK_6MHZ:
      if(IMX219_WriteTable(pObj, inck_6Mhz_regs, ARRAY_SIZE(inck_6Mhz_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    case IMX219_INCK_24MHZ:
      if(IMX219_WriteTable(pObj, inck_24Mhz_regs, ARRAY_SIZE(inck_24Mhz_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    case IMX219_INCK_27MHZ:
      if(IMX219_WriteTable(pObj, inck_27Mhz_regs, ARRAY_SIZE(inck_27Mhz_regs)) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
      break;
    default:
      ret = IMX219_ERROR;
      break;
  };

  return ret;
}

/**
  * @brief  Set the frame rate
  * @param  pObj  pointer to component object
  * @param  framerate Target frame rate
  * @retval Component status
  */
int32_t IMX219_SetFramerate(IMX219_Object_t *pObj, int32_t framerate)
{
  /* Frame rate is controlled by frame length register configuration 
     which is already set in resolution register tables */
  (void)pObj;
  (void)framerate;
  
  return IMX219_OK;
}

/**
  * @brief  Control imx219 camera mirror/vflip.
  * @param  pObj  pointer to component object
  * @param  Config To configure mirror, flip, both or none
  * @retval Component status
  */
int32_t IMX219_MirrorFlipConfig(IMX219_Object_t *pObj, uint32_t Config)
{
  int32_t ret = IMX219_OK;

  switch (Config)
  {
    case IMX219_FLIP:
      ret = IMX219_WriteTable(pObj, mirrorflip_mode_regs[1], ARRAY_SIZE(mirrorflip_mode_regs[1]));
      break;
    case IMX219_MIRROR:
      ret = IMX219_WriteTable(pObj, mirrorflip_mode_regs[2], ARRAY_SIZE(mirrorflip_mode_regs[2]));
      break;
    case IMX219_MIRROR_FLIP:
      ret = IMX219_WriteTable(pObj, mirrorflip_mode_regs[3], ARRAY_SIZE(mirrorflip_mode_regs[3]));
      break;
    case IMX219_MIRROR_FLIP_NONE:
    default:
      ret = IMX219_WriteTable(pObj, mirrorflip_mode_regs[0], ARRAY_SIZE(mirrorflip_mode_regs[0]));
      break;
  }
  return ret;
}

/**
  * @brief  Set the Test Pattern Generator
  * @param  pObj  pointer to component object
  * @param  mode Pattern mode:
  *              -1 : Disable
  *               0 : Color bars
  *               1 : Solid color
  *               2 : Grey scale bars
  *               3 : PN9 
  * @retval Component status
  */
int32_t IMX219_SetTestPattern(IMX219_Object_t *pObj, int32_t mode)
{
  int32_t ret = IMX219_OK;
  uint8_t val[2];

  if (mode >= 0)
  {
    /* Enable Test Pattern #mode */
    val[0] = (uint8_t)((mode >> 8) & 0xFF);
    val[1] = (uint8_t)(mode & 0xFF);
    if (imx219_write_reg(&pObj->Ctx, IMX219_REG_TEST_PATTERN, val, 2) != IMX219_OK)
    {
      return IMX219_ERROR;
    }
    if (IMX219_WriteTable(pObj, test_pattern_enable_regs, ARRAY_SIZE(test_pattern_enable_regs)) != IMX219_OK)
    {
      return IMX219_ERROR;
    }
  }
  else
  {
    /* Disable Test Pattern */
    if (IMX219_WriteTable(pObj, test_pattern_disable_regs, ARRAY_SIZE(test_pattern_disable_regs)) != IMX219_OK)
    {
      return IMX219_ERROR;
    }
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