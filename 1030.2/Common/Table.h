/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:Table.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 三角函数查找表
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History
*
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2021-08-17      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef TABLE_H
#define TABLE_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
#define TABLE_SIZE_SIN_COS (1024u)
#define TABLE_CFG_PWM_OUT_AMP_TABLE_SCALE (0.117553711)

/* Variables */
/******************************************************************/
extern const s16 Table_Sin[];
extern const u16 Table_ArcTan[];
extern const u16 Table_Amp[];
extern const u16 Table_Sin60[];
extern const s16 *pTable_Cos;

#endif /* TABLE_H */
