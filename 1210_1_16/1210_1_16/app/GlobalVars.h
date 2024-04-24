/*
 * @Author: your name
 * @Date: 2021-07-20 11:42:55
 * @LastEditTime: 2021-07-23 19:57:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \110WPump\app\GlobalVars.h
 */
/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:GlobalVars.h
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
#ifndef GlobalVars_H
#define GlobalVars_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"

/* Defines*/
/******************************************************************/
#define  POWERUP_TIME_100MS (100u)
/* 0xFFFFFFFFu - 100u  -2021-08-04-by zyg */
#define MAX_POWERUP_TIME_MS (0xFFFFFFFFu - POWERUP_TIME_100MS)

/*polyspace-begin  MISRA2012:6.1*/
typedef struct {
/*增加FCT版本号，24-02-23，by WChuang*/
  u16 ProductType;
  union{
    u16 reg;

    struct{
      u16 MinorHardVersion:4;
      u16 InnerVersion:4;
      u16 SoftVersion:8;
    }bits;
  }InnerVersion;
	u8 FctVersion;
  u8 DebugVersion;
	 /* 电机库版本，对于有电机库的代码，代表所使用电机库的版本 */
	u8 McLib;
}Version_t;
/*polyspace-end MISRA2012:6.1*/
/* Variables */
/******************************************************************/
extern const Version_t ProductVersion;
extern u32 PowerUpTimeMs;

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name: GlobalVars_PowerUpTimeCount
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
********************************************************************
END_FUNCTION_HDR*/
static inline void GlobalVars_PowerUpTimeCount(void)
{
	/* 宏代替，2021-08-18-by zyg*/
	if(PowerUpTimeMs < (MAX_POWERUP_TIME_MS))
	{
		PowerUpTimeMs += POWERUP_TIME_100MS;
	}
	else/* add else - 2021-07-28-by zyg*/
	{
		
	}
}	

#endif
