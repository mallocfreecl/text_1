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

/* Includes*/
/******************************************************************/
#include "SdkCfg.h"

#include "tle_device.h"

/* Defines*/
/******************************************************************/
#define RAM_START_ADDR (RAMStart)
#define RAM_END_ADDR ((RAMStart) + (RAMSize))

#define FLASH_CHECK_SUM_AND_BAR_CODE_SIZE (0x18u)
#define CFLASH_ADDR_START (ProgFlashStart)
#define CFLASH_ADDR_END ((DataFlashStart) - (FLASH_CHECK_SUM_AND_BAR_CODE_SIZE))

/* Variables */
/******************************************************************/
const SdkCfg_t SdkCfg = {
  /*点火信号唤醒 22.04.11 by jxj*/
  .WakeUpSrc = eWakeUpSrcPwmIn,

  /* 1.25V, secured OpAmp differential input voltage range */
  /* 1.25V/20/0.001Ohm= 62.5A*/
  .IShuntGain = eIShuntGain20,

  .RamStartAddr = RAM_START_ADDR,
  .RamEndAddr = RAM_END_ADDR,

  .CFlashStartAddr = CFLASH_ADDR_START,
  .CFlashEndAddr = CFLASH_ADDR_END,

  .DFlashStartAddr = DataFlashStart,

  /*增加备份存储区地址 22.04.by jxj*/
  .DFlashEndAddr = DataFlashStart + DataFlashSize,
  .DFlashBackUpAddr = DataFlashStart + (DataFlashSize >> 1u),
  .PwmOutDefaultSts = 1u,
};

/* 每次修改程序后必须修改该值, 23.05.13 by yhd  */
/*修改校验和 22.06.08 by jxj*/
const u32 CFlashChkSumStored __attribute__((section(".ARM.__at_0x11007fe8"))) = 0x0000F8D7;
