/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:UserInfo.c
********************************************************************
* Project/Product:LXT9820_PMP
* Title:
* Author:zyg
*********************************************************************
* Description:
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
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-09-06      zyg            Original
*	
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "UserInfo.h"

/* Variables */
/******************************************************************/
const u32 FlashCheckSum = 0x8161ECD5uL;/*  Modify CheckSum - 2023-03-26-by lxs*/
const u8 FlashECUInfo[12u] = {0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0x01u,0x01u};
