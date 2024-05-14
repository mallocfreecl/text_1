/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:FanComTest.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 和上位机Fan_Com_Test.exe配合使用的串口调试功能。
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
#ifndef FanComTest_H
#define FanComTest_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
/* Receive data from PC.Used for debug */
#define FAN_COM_TEST_RX_ID_UART_DEBUG (0x10u)
/* Transmition data to PC */
#define FAN_COM_TEST_TX_ID_UART_DEBUG (0x20u)
/* Receive data from PC.Used for fct  */
#define FAN_COM_TEST_RX_ID_ENTRY_FCT_TEST_MODE (0x01u)
/* Receive data from PC.Used for fct */
#define FAN_COM_TEST_RX_ID_TEST_MOS (0x03u)
/* Receive data from PC.Used for fct */
#define FAN_COM_TEST_RX_ID_TEST_PWM_OUT (0x05u)
#define FAN_COM_TEST_RX_ID_TEST_SLEEP (0x07u)

#define FAN_COM_TEST_TX_ID_ENTRY_FCT_TEST_MODE (0x02u)
#define FAN_COM_TEST_TX_ID_TEST_MOS (0x04u)
#define FAN_COM_TEST_TX_ID_TEST_PWM_OUT (0x06u)

/* Function declaration */
/******************************************************************/
extern void FanComTest_writeU8(u8 signum, u8 value);
extern u8 FanComTest_readU8(u8 signum);
extern void FanComTest_rstU8(u8 signum);
extern void FanComTest_writeU16(u8 signum, u16 value);
extern void FanComTest_writeS16(u8 signum, s16 value);
extern u16 FanComTest_readU16(u8 signum);
extern u8 FanComTest_getRxID(void);
extern void FanComTest_startFrameTx(u8 FrameId);

/* 在串口收发中断中调用下面函数 */
extern void FanComTest_rxHandler(void);
extern void FanComTest_txHandler(void);

#endif
