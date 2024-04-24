/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:UserInfo.c
********************************************************************
* Project/Product:LXT9818_PMP
* Title:
* Author:yhd
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
*     1.0       2021-07-23      yhd            Original
*			1.0       2021-08-02      zyg            1.Modify FlashCheckSum
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "UserInfo.h"

/* Variables */
/******************************************************************/
const u32 FlashCheckSum = 0x04548900u;/*  Modify CheckSum - 2024-03-02-by WChuang*/
const u8 FlashECUInfo[12u] = {0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0xFFu,0x01u,0x01u};
