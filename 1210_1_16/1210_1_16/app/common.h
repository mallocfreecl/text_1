/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:Common.h
********************************************************************
* Project/Product:LXT9818
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
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef COMMON_H
#define COMMON_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"

/* Defines*/
/******************************************************************/
#define ABS(x) (((x) >= 0) ? (x) : (-(x)))

/* Function declaration */
/******************************************************************/
extern s16 Common_ExeLookUpTable(s16 u0, const s16 bp0[], const
  s16 table[], u32 maxIndex);
extern void Common_CopyMem(u8 des[],u8 src[],u8 length);
extern u8 Common_SetStatusDelay(u8 status,u16 filtertime,u16 *currenttime);

#endif
