/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:GlobalVars.c
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

/* Includes*/
/******************************************************************/
#include "GlobalVars.h"

/* Variables */
/******************************************************************/
/* 升级版本号，23.05.13 by yhd */
const Version_t ProductVersion = {
  0x36740000u,

  {
    .bits =
      {
        0x0u,
        0x3u,
        0x20u,
      },
  },

  1u,
};

u32 PowerUpTimeMs = 0u; // Power up time in microseconds

/* 条码内容在下载程序时由烧写器烧入 */
const u8 BarCode[12u] __attribute__((section(".ARM.__at_0x11007fec"))) = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                                                                          0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x01u, 0x01u};
