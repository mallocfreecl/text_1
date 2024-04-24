/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:GlobalVars.c
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
*     1.0       2021-09-06        zyg            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "GlobalVars.h"

/* Variables */
/******************************************************************/
/*消除polyspace警告,2021-09-22-by zyg*/
/*polyspace-begin  MISRA2012:9.2*/
const Version_t ProductVersion = 
{
	/*修改软件版本,22.10.18 by zjj*/
.ProductType = 0x98200000u,

.InnerVersion.bits.MinorHardVersion = 0u,
.InnerVersion.bits.InnerVersion = 7u,
.InnerVersion.bits.SoftVersion = 0x13u,
	/*新增硬件号,22.05.16 by zyg*/
.MajorHardVersion = 1u,
.DebugVersion = 1u,/*  升级调试版本号,23.03.26 by lxs  */
};
/*polyspace-end  MISRA2012:9.2*/
u32 PowerUpTimeMs = 0u; /* Power up time in microseconds */
