/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:GlobalVars.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.定义软件型号和版本号
* 2.提供上电计时
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History：
*
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2021-12-09      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef GlobalVars_H
#define GlobalVars_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* unit:ms */
#define GLOBAL_VAR_POWER_UP_TIME_BASE (100u)

/* Defines*/
/******************************************************************/
/* polyspace-begin MISRA-C3:6.1 */
typedef struct
{
  u32 ProductType;
  union {
    u16 reg;

    struct
    {
      u16 MinorHardVersion : 4;
      u16 InnerVersion : 4;
      u16 SoftVersion : 8;
    } bits;
  } InnerVersion;
  u8 DebugVersion;
} Version_t;
/* polyspace-end MISRA-C3:6.1 */

/* Variables */
/******************************************************************/
extern const Version_t ProductVersion;
extern u32 PowerUpTimeMs;

extern const u8 BarCode[];

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name: GlobalVars_updatePowerUpTimeCnt
* Description:
*   Record powerup time in milliseconds
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.必须在GLOBAL_VAR_POWER_UP_TIME_BASE设置的周期调用该函数
********************************************************************
END_FUNCTION_HDR*/
STATIC_INLINE void GlobalVars_updatePowerUpTimeCnt(void)
{
  if (PowerUpTimeMs < (U32_MAX - GLOBAL_VAR_POWER_UP_TIME_BASE))
  {
    PowerUpTimeMs += GLOBAL_VAR_POWER_UP_TIME_BASE;
  }
}

#endif
