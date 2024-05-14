/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:SdkCfg.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.对Sdk中的可选项进行配置
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
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ---------------------
*     1.0       2022-01-29      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef SDK_CFG
#define SDK_CFG

/* Includes*/
/******************************************************************/
#include "Sdk.h"

/* Defines*/
/******************************************************************/
typedef enum
{
  /* 设置PwmIn作为唤醒源 */
  eWakeUpSrcPwmIn = 0x1u,
  /* 设置IG作为唤醒源 */
  eWakeUpSrcIG = 0x2u,
  /* 设置PwmIn和IG同时作为唤醒源 */
  eWakeUpSrcIGAndPwmIn = 0x3u,
} WakeUpSrc_t;

typedef enum
{
  eIShuntGain10 = 0,
  eIShuntGain20 = 1,
  eIShuntGain40 = 2,
  eIShuntGain60 = 3,
} IShuntGain_t;

typedef struct
{
  WakeUpSrc_t WakeUpSrc;
  IShuntGain_t IShuntGain;

  u32 RamStartAddr;
  u32 RamEndAddr;

  u32 CFlashStartAddr;
  u32 CFlashEndAddr;

  u32 DFlashStartAddr;
  u32 DFlashEndAddr;
  /*EEROM备份地址 22.04.23 by jxj */
  u32 DFlashBackUpAddr;

  u8 PwmOutDefaultSts;
} SdkCfg_t;

/* Variables */
/******************************************************************/
extern const SdkCfg_t SdkCfg;
extern const u32 CFlashChkSumStored;

#endif // !SDK_CFG
