/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:FanComTest.h
********************************************************************
* Project/Product:LXT9818
* Title:
* Author:yhd
*********************************************************************
* Description:
*		Exchange data between ECU and Fan_Com_Test.exe.
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
#ifndef FanComTest_H
#define FanComTest_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"

/* Defines*/
/******************************************************************/
#define RX_ID_UART_DEBUG       (0x10u) /* Receive data from PC.Used for debug. */
#define TX_ID_UART_DEBUG       (0x20u) /* Transmition data to PC. */
#define RX_ID_ENTRY_FCT_TEST_MODE                 (0x01u) /* Receive data from PC.Used for fct. */
#define RX_ID_TEST_MOS                 (0x03u) /* Receive data from PC.Used for fct. */
#define RX_ID_TEST_PWM_OUT                 (0x05u) /* Receive data from PC.Used for fct. */

#define TX_ID_ENTRY_FCT_TEST_MODE (0x02u) 
#define TX_ID_TEST_MOS (0x04u) 
#define TX_ID_TEST_PWM_OUT (0x06u) 

/* Function declaration */
/******************************************************************/
extern void FanComTest_WriteU8(u8 signum,u8 value);
extern u8 FanComTest_ReadU8(u8 signum);
extern void FanComTest_ResetU8(u8 signum);
extern void FanComTest_WriteU16(u8 signum,u16 value);
extern void FanComTest_WriteS16(u8 signum,s16 value);
extern u16 FanComTest_ReadU16(u8 signum);
extern u8 FanComTest_GetRxID(void);
extern void FanComTest_StartFrameTransmission(u8 FrameId);

extern void FanComTest_RxHandler(void);
extern void FanComTest_TxHandler(void);

#endif
