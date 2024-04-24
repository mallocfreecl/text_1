/* BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name: LINDataPro.c
********************************************************************
* Project/Product: LXT9820_PMP
* Title: lin收发数据处理
* Author: zyg
*********************************************************************
* Description:
*	此文件用于LIN收发数据的处理
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
* Version      Date              Author         Descriptions
* ----------   --------------    ------------   ----------------------------------------
*  1.0         2021-12-28        zyg            Original
*
*
********************************************************************
*END_FILE_HDR */

/******************************************************************/
/* file include */
/******************************************************************/
#include "LINDataPro.h"
#include "lin_cfg.h"
#include "lin_sw.h"
#include "lin_lld_sci.h"
#include "common.h"
#include "MotorAPI.h"
#include "ADCSample.h"
#include "AppMain.h"
#include "rtwtypes.h"
/******************************************************************/
/* MACRO DEFINE */
/******************************************************************/
/*存在故障反馈延迟置1,2021-09-06-by zyg*/
#define DELAY_ENABILE 0
#define LIN_BUS_ACTIVITY_MASK (1<<4)
#define LIN_GO_TO_SLEEP_MASK  (1<<3)
#define Tx_Time_Base 50 //ms
#define Motor_Short_Feedback_Delay_Time (0*1000/Tx_Time_Base)  //s
#define Motor_Lock_Feedback_Delay_Time  (0*1000/Tx_Time_Base)  //s
#define Motor_Open_Feedback_Delay_Time  (0*1000/Tx_Time_Base)  //s
/*第二次故障开始反馈故障 22.09.29 by lxs*/
#define Feedback_Fault_Cnt 2u
/*报干运行故障时间 22.09.29 by lxs*/
#define Dryrunning_Feedback_Fault_Time 4000u
/******************************************************************/
/* enum  */
/******************************************************************/
enum {
  eNormal = 0,

  ePreOverTemp = 1,
  eOverTemp = 2,
  eLowTemp = 3,

  eMotorShort = 1,
  eMotorOverCurrent = 2,

  eMotorDryRunning = 1,
  eMotorDryStop = 2,

  eVoltageHigh = 1,
  eVoltageLow = 2,

  eMotorBlock = 1,
};
/******************************************************************/
/* static variables */
/******************************************************************/
#if DELAY_ENABILE
static struct {
  struct{
    uint16 Short;
    uint16 Open;
    uint16 Lock;
  }Motor;
} FeedBackDelayVar ={{0,0,0}};
#endif
/******************************************************************/
/* global variables */
/******************************************************************/
LinRx_t LinRx;
LinTx_t LinTx;
/******************************************************************/
/* static funciton */
/******************************************************************/
/****************************************************************
* FUNCTION :  LIN_Rx
* DESCRIPTION : LIN接收函数
* INPUTS :  None
* OUTPUTS : None
* Limitations:50ms任务
****************************************************************/
void LIN_Rx(void)
{
  LinRx.IsSleep = (bool)(lin_lld_sci_get_state() == SLEEP_MODE);
  LinRx.SpdSet = (uint16)l_u8_rd_LI0_BEWP_RefSpd();
	/*若接收值不为255，则更新转速设置，否则转速按照上次转速更新；上电时接收到255则直接按照默认值(0)更新,22.05.13 by zyg*/
	if(LinRx.SpdSet != 255u)
	{
		LinRx.SpdSetLast = LinRx.SpdSet;
	}
}
/****************************************************************
* FUNCTION :  LIN_Tx
* DESCRIPTION : LIN发送函数
* INPUTS :  None
* OUTPUTS : None
* Limitations:50ms任务	
****************************************************************/
void LIN_Tx(void)
{
  uint8 MotorShort = 0;
  uint8 MotorOpen = 0;
  uint8 MotorLock = 0;
	/*消除polyspace警告,2021-09-22-by zyg*/
	/*修改为重启第二次开始反馈故障 22.09.25 by lxs*/
  if((MotorAPIVar.Faults.bit.Short == 1) && (Y.ShortCnt >= Feedback_Fault_Cnt))
  {
#if DELAY_ENABILE
    if(FeedBackDelayVar.Motor.Short >= Motor_Short_Feedback_Delay_Time)
    {
      MotorShort = 1;
    }
    else
    {
      FeedBackDelayVar.Motor.Short++;
    }
#else
		MotorShort = 1;
#endif
  }
  else
  {
    MotorShort = 0;
		
#if DELAY_ENABILE
    FeedBackDelayVar.Motor.Short = 0;
#endif
		
  }
	/*消除polyspace警告,2021-09-22-by zyg*/
	/*修改为重启第二次开始反馈故障 22.09.25 by lxs*/
  if((MotorAPIVar.Faults.bit.Open == 1) && (Y.OpenCnt >= Feedback_Fault_Cnt))
  {
#if DELAY_ENABILE	
    if(FeedBackDelayVar.Motor.Open >= Motor_Open_Feedback_Delay_Time)
    {
      MotorOpen = 1;
    }
    else
    {
      FeedBackDelayVar.Motor.Open++;
    }
#else
		MotorOpen = 1;
#endif
  }
  else
  {
    MotorOpen = 0;

#if DELAY_ENABILE
    FeedBackDelayVar.Motor.Open = 0;
#endif
  }
	/*消除polyspace警告,2021-09-22-by zyg*/
	/*修改为直接开始反馈故障 22.09.25 by lxs*/
  if(MotorAPIVar.Faults.bit.Lock == 1)
  {
#if DELAY_ENABILE	
    if(FeedBackDelayVar.Motor.Lock >= Motor_Lock_Feedback_Delay_Time)
    {
      MotorLock = 1;
    }
    else
    {
      FeedBackDelayVar.Motor.Lock++;
    }
#else
		MotorLock = 1;
#endif
  }
  else
  {
    MotorLock = 0;
		
#if DELAY_ENABILE
    FeedBackDelayVar.Motor.Lock = 0;
#endif
		
  }

  LinTx.Sts.bit.OverCurrent = eNormal;
  LinTx.Sts.bit.Block = eNormal;

  if(MotorShort != 0u)
  {
    LinTx.Sts.bit.OverCurrent = eMotorShort;
  }
	else
	{
		
	}
	if(MotorOpen == 1u)
  {
    
  }
	else
	{
		
	}
  if(MotorLock == 1u)
  {
    if( (MotorAPIVar.DetailFaults.bit.MotorOnOverCurrent) != 0u)
    {
	/*修改为重启第二次开始反馈故障 22.09.25 by lxs*/
			if(Y.LockCnt >= Feedback_Fault_Cnt)
			{
			  LinTx.Sts.bit.OverCurrent = eMotorOverCurrent;	
			}
			else
			{
				
			}
    }
    else
    {
      LinTx.Sts.bit.Block = eMotorBlock;
    }
  }
	else
	{
		
	}
  
  if((MotorAPIVar.IsDryRunningDetected) == 1u)
  {
    if((MotorAPIVar.IsDryRunning) != 0u)
    {
      LinTx.Sts.bit.DryRunning = eMotorDryStop;
    }
    else
    {
			if(MotorAPIVar.DryRunningCurrentLowTime > Dryrunning_Feedback_Fault_Time)
			{
				LinTx.Sts.bit.DryRunning = eMotorDryRunning;
			}
			else
			{
				LinTx.Sts.bit.DryRunning = eNormal;
			}
    }  
  }
  else
  {
  /*修改为干运行电流持续低于4s后反馈故障 22.09.25 by lxs*/
    if(MotorAPIVar.DryRunningCurrentLowTime > Dryrunning_Feedback_Fault_Time)  
    {
      LinTx.Sts.bit.DryRunning = eMotorDryRunning;
    }
    else
    {
      LinTx.Sts.bit.DryRunning = eNormal;
    }
  }

  if(Y.VoltageStatus == -1)
  {
    LinTx.Sts.bit.Voltage = eVoltageLow;
  }
  else if(Y.VoltageStatus == 1)
  {
    LinTx.Sts.bit.Voltage = eVoltageHigh;
  }
  else
  {
    LinTx.Sts.bit.Voltage = eNormal;
  }
	
	if(Y.AmbTStatus == 1)
  {
    LinTx.Sts.bit.OverTemp = eOverTemp;
  }
  else if (Y.AmbTStatus == -1)
  {
    LinTx.Sts.bit.OverTemp = eLowTemp;
  }
  else if (Y.AmbTPreOverStatus == 1)
  {
    LinTx.Sts.bit.OverTemp = ePreOverTemp;
  }
  else
  {
    LinTx.Sts.bit.OverTemp = eNormal;
  }
	//MotorAPIVar.RefSpeed/5800/0.4/0.01
	/*polyspace-begin  MISRA2012:10.8*/
	LinTx.TargetSpd = (u8)(((s32)MotorAPIVar.RefSpeed) * 1000 / MOTOR_MAX_SPEED / 4);
	/*polyspace-end  MISRA2012:10.8*/
	if(MotorAPIVar.ActSpeed > MOTOR_FEEDBACK_MAX_SPEED)
	{
		LinTx.ActSpd = 255;
	}
	else if(MotorAPIVar.ActSpeed < 0)
	{
		LinTx.ActSpd = 0;
	}
	else 
	{
		/*polyspace-begin  MISRA2012:10.8*/
		LinTx.ActSpd = (u8)(((s32)MotorAPIVar.ActSpeed) * 1000 / MOTOR_MAX_SPEED / 4);
		/*polyspace-end  MISRA2012:10.8*/
	}
  if(ADCResults.Voltage.Bat > 0xFFu)
  {
    LinTx.Voltage = 0xFFu;
  }
  else
  {
    LinTx.Voltage = ADCResults.Voltage.Bat;
  }
  
  if(ADCResults.Temperature.Chip < -50)
  {
    LinTx.Temperature = 0;
  }
	else if(ADCResults.Temperature.Chip > 205)
	{
		LinTx.Temperature = 0xFF;
	}
  else
  {
    LinTx.Temperature = ADCResults.Temperature.Chip + 50;/* polyspace DEFECT:UINT_CONV_OVFL */
  }
  
  if(MotorAPIVar.IShunt < 0)
  {
    LinTx.Current = 0;
  }
  else if(MotorAPIVar.IShunt > 255)
  {
    LinTx.Current = 255;
  }
  else
  {
    LinTx.Current = MotorAPIVar.IShunt;
  }	
	
  l_u8_wr_LI0_BEWP_TargetSpd(LinTx.TargetSpd);
  l_u8_wr_LI0_BEWP_ActSpd(LinTx.ActSpd);
  l_u8_wr_LI0_BEWP_Voltage(LinTx.Voltage);
  l_u8_wr_LI0_BEWP_Temperature(LinTx.Temperature);
  l_u8_wr_LI0_BEWP_Current(LinTx.Current);
  l_u8_wr_LI0_BEWP_OverTempSts(LinTx.Sts.bit.OverTemp);
  l_u8_wr_LI0_BEWP_OverCurrentSts(LinTx.Sts.bit.OverCurrent);
  l_u8_wr_LI0_BEWP_DryRunningSts(LinTx.Sts.bit.DryRunning);
  l_u8_wr_LI0_BEWP_VoltageSts(LinTx.Sts.bit.Voltage);
  l_u8_wr_LI0_BEWP_BlockSts(LinTx.Sts.bit.Block);
}
