/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:LINDataPro.h
********************************************************************
* Project/Product:LXT9820_PMP
* Title :
* Author:zyg
*********************************************************************
* Description:
* ͷ�ļ�
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History��
*
* Version       Date         	  Author         Descriptions
* ----------    --------------    ------------   ----------------------------------------
*  1.0          2021-09-06        zyg            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#ifndef LIN_DATA_PRO_H
#define LIN_DATA_PRO_H
/******************************************************************/
/* file include */
/******************************************************************/
#include "AppTypes.h"
/******************************************************************/
/* TYPE DEFINE */
/******************************************************************/

/*水泵运行最大转速 23.03.26 by lxs*/
#define MOTOR_MAX_SPEED (5800)
/*水泵LIN反馈最大转速 23.02.15 by lxs*/
#define MOTOR_FEEDBACK_MAX_SPEED (MOTOR_MAX_SPEED*255/250)

typedef struct{
  u16 SpdSet;
  u8 Enable;
  u8 IsSleep;
	/*记录上次转速设置值*/
	u16 SpdSetLast;
}LinRx_t;
/*消除polyspace警告,2021-09-22-by zyg*/
/*polyspace-begin  MISRA2012:6.1*/
typedef struct{
  u8 TargetSpd;
  u8 ActSpd;
  u8 Voltage;
  u8 Temperature;
  u8 Current;

  union{
    u16 reg;

    struct{
      u16 OverTemp:2;
      u16 OverCurrent:2;
      u16 DryRunning:2;
      u16 Voltage:2;
      u16 Block:2;
    }bit;
  }Sts;
}LinTx_t;
/*polyspace-end  MISRA2012:6.1*/
/******************************************************************/
/* global variables declaration */
/******************************************************************/
extern LinRx_t LinRx;
extern LinTx_t LinTx;
/******************************************************************/
/* function declaration */
/******************************************************************/
extern void LIN_Rx(void);
extern void LIN_Tx(void);

#endif
