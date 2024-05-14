/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:FanComTest.c
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

/* Includes*/
/******************************************************************/
#include "FanComTest.h"
#include "Common.h"
#include "Sdk.h"

/* Defines*/
/******************************************************************/
#define FAN_COM_TEST_DATA_BUF_LEN 8u

#define FAN_COM_TEST_RECEIVE_ID_STATE 0u
#define FAN_COM_TEST_CHECK_ID_STATE 1u
#define FAN_COM_TEST_SEND_DATA_STATE 2u
#define FAN_COM_TEST_RECEIVE_DATA_STATE 3u
#define FAN_COM_TEST_SEND_COMPLETE_STATE 4u
#define FAN_COM_TEST_CHECK_CHKSUM_STATE 5u
#define FAN_COM_TEST_SEND_LASTCHK_STATE 6u
#define FAN_COM_TEST_SEND_ID_STATE 7u
#define FAN_COM_TEST_SEND_IDLE_STATE 9u

#define FAN_COM_TEST_TIMEOUT_CNT_EN 1u
#define FAN_COM_TEST_TIMEOUT_CNT_DIS 0u

#define FAN_COM_TEST_U16_DATA_OFFSET (32768u)

typedef enum
{
  IDErr,
  IDCheckErr,
  ChecksumErr,
  TimeoutErr
} FanComTestErr_t;

typedef struct
{
  u8 id;
  u8 state;
  u8 length;
  u8 cnt;
  u8 buffer[FAN_COM_TEST_DATA_BUF_LEN];
  u8 chksumbuffer[2u];
  u16 checksum;
  u8 timeoutcnt;
  u8 timeouten;
} _uart_state_machine;

/* Variables */
/******************************************************************/
static _uart_state_machine Uart_RxState_Machine;
static _uart_state_machine Uart_TxState_Machine;

static u8 uart_sig_ttbl[FAN_COM_TEST_DATA_BUF_LEN];
static u8 uart_sig_rtbl[FAN_COM_TEST_DATA_BUF_LEN];
static u8 uart_sig_id;

/* Function declaration */
/******************************************************************/
static void FanComTest_proUartErr(FanComTestErr_t errtype);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_writeU8
* Description:
*		Write data to send buffer.Call FanComTest_txHandler() to start
trans-
*   mission.The Fan_Com_Test.exe received data is dipaly in
*   READ DATA array.The first two bytes and last two bytes is not
*   used for user data, so only eight data between is valid.
*
* Inputs:
*	1.signum:0~7
* 2.value:data
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void FanComTest_writeU8(u8 signum, u8 value)
{
  if (signum < FAN_COM_TEST_DATA_BUF_LEN)
  {
    uart_sig_ttbl[signum] = value;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_readU8
* Description:
*		Read data buffer which is buffered data from the PC.
*
* Inputs:
*	1.signum:0~7
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 FanComTest_readU8(u8 signum)
{
  u8 ret = 0u;

  if (signum < (FAN_COM_TEST_DATA_BUF_LEN))
  {
    ret = uart_sig_rtbl[signum];
  }
  else
  {
    ret = U8_MAX;
  }

  return ret;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:signum
* Description:
*		Clear receive buffer
*
* Inputs:
*	1.signum:0~7
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void FanComTest_rstU8(u8 signum)
{
  if (signum < FAN_COM_TEST_DATA_BUF_LEN)
  {
    uart_sig_rtbl[signum] = 0x00u;
  }
  else
  {
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_writeU16
* Description:
*		Write U16 data to transmition buffer.
*
* Inputs:
* 1.signum:0~3
* 2.value:data value
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void FanComTest_writeU16(u8 signum, u16 value)
{
  signum = (signum << 1u) + 1u;
  if (signum < (FAN_COM_TEST_DATA_BUF_LEN))
  {
    uart_sig_ttbl[signum - 1u] = (value + FAN_COM_TEST_U16_DATA_OFFSET) >> 8u;
    uart_sig_ttbl[signum] = (value + FAN_COM_TEST_U16_DATA_OFFSET) & U8_MAX;
  }
  else
  {
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_writeS16
* Description:
*		Write S16 data to transmition buffer.
*
* Inputs:
* 1.signum:0~3
* 2.value:data value
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void FanComTest_writeS16(u8 signum, s16 value)
{
  u16 tmp = 0u;

  signum = (signum << 1u) + 1u;
  if (signum < (FAN_COM_TEST_DATA_BUF_LEN))
  {
    /* polyspace +1 MISRA-C3:10.8 */
    tmp = (u16)((s32)value + (s32)FAN_COM_TEST_U16_DATA_OFFSET);
    uart_sig_ttbl[signum - 1u] = tmp >> 8u;
    uart_sig_ttbl[signum] = tmp;
  }
  else
  {
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_readU16
* Description:
*		Read U16 data from receive buffer.
*
* Inputs:
* 1.signum:0~3
*
* Outputs:
* 1.u16 data
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u16 FanComTest_readU16(u8 signum)
{
  u16 ret = U16_MAX;
  signum = (signum << 1u) + 2u;
  if ((signum < FAN_COM_TEST_DATA_BUF_LEN) && (signum >= 1))
  {
    ret = (((u16)uart_sig_rtbl[signum - 1u] << 8u) | (u16)uart_sig_rtbl[signum]);
  }

  return ret;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_getRxID
* Description:
*		Get ID from receive data.
*
* Inputs:
*
*
* Outputs:
* 1.u8 ID
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 FanComTest_getRxID(void)
{
  return uart_sig_id;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_proUartErr
* Description:
*		Reset uart state machine
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
static void FanComTest_proUartErr(FanComTestErr_t errtype)
{
  Uart_RxState_Machine.timeouten = FAN_COM_TEST_TIMEOUT_CNT_DIS;
  Uart_RxState_Machine.timeoutcnt = 0u;
  Uart_RxState_Machine.state = FAN_COM_TEST_RECEIVE_ID_STATE;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_rxHandler
* Description:
*		Call this function in uart byte rx completed handler
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
void FanComTest_rxHandler(void)
{
  u8 temp;
  temp = Sdk_readUartU8();

  Uart_RxState_Machine.timeouten = FAN_COM_TEST_TIMEOUT_CNT_EN;
  Uart_RxState_Machine.timeoutcnt = 0u;

  switch (Uart_RxState_Machine.state)
  {
  case FAN_COM_TEST_RECEIVE_ID_STATE: {
    switch (temp)
    {
    case FAN_COM_TEST_RX_ID_UART_DEBUG:
    case FAN_COM_TEST_TX_ID_UART_DEBUG:
    case FAN_COM_TEST_RX_ID_ENTRY_FCT_TEST_MODE:
    case FAN_COM_TEST_RX_ID_TEST_MOS:
    case FAN_COM_TEST_RX_ID_TEST_PWM_OUT:
    case FAN_COM_TEST_RX_ID_TEST_SLEEP:
      // No need "break" for above cases.
      Uart_RxState_Machine.id = temp;
      Uart_RxState_Machine.state = FAN_COM_TEST_CHECK_ID_STATE;
      break;
    default:
      FanComTest_proUartErr(IDErr);
      break;
    }
  }
  break;
  case FAN_COM_TEST_CHECK_ID_STATE: {
    u8 antitemp;
    antitemp = (~temp);
    if (Uart_RxState_Machine.id == antitemp)
    {
      Uart_RxState_Machine.state = FAN_COM_TEST_RECEIVE_DATA_STATE;
      Uart_RxState_Machine.length = FAN_COM_TEST_DATA_BUF_LEN;
      Uart_RxState_Machine.cnt = 0u;
      Uart_RxState_Machine.checksum = 0u;
    }
    else
    {
      FanComTest_proUartErr(IDCheckErr);
    }
  }
  break;
  case FAN_COM_TEST_RECEIVE_DATA_STATE: {
    Uart_RxState_Machine.buffer[Uart_RxState_Machine.cnt] = temp;
    Uart_RxState_Machine.checksum += temp;
    Uart_RxState_Machine.cnt++;
    if (Uart_RxState_Machine.cnt >= Uart_RxState_Machine.length)
    {
      Uart_RxState_Machine.state = FAN_COM_TEST_CHECK_CHKSUM_STATE;
      Uart_RxState_Machine.cnt = 0u;
    }
    else
    {
    }
  }
  break;
  case FAN_COM_TEST_CHECK_CHKSUM_STATE: {
    Uart_RxState_Machine.chksumbuffer[Uart_RxState_Machine.cnt] = temp;
    Uart_RxState_Machine.cnt++;
    if (Uart_RxState_Machine.cnt >= 2u)
    {
      u16 chksum = 0u;
      chksum = ((u16)Uart_RxState_Machine.chksumbuffer[0u] << 8u) | Uart_RxState_Machine.chksumbuffer[1u];
      if (chksum == Uart_RxState_Machine.checksum)
      {
        uart_sig_id = Uart_RxState_Machine.id;
        Common_cpMemory(uart_sig_rtbl, Uart_RxState_Machine.buffer, Uart_RxState_Machine.length);
        Uart_RxState_Machine.state = FAN_COM_TEST_RECEIVE_ID_STATE;
        Uart_RxState_Machine.timeouten = FAN_COM_TEST_TIMEOUT_CNT_DIS;
        Uart_RxState_Machine.timeoutcnt = 0u;
      }
      else
      {
        FanComTest_proUartErr(ChecksumErr);
      }
    }
    else
    {
    }
  }
  break;
  default: /* Not use */
    break;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_txHandler
* Description:
*		Call this function in uart byte tx completed handler
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
void FanComTest_txHandler(void)
{
  switch (Uart_TxState_Machine.state)
  {
  case FAN_COM_TEST_SEND_ID_STATE: {
    u8 temp;
    temp = Uart_TxState_Machine.id;
    (void)Sdk_writeUartU8(~temp);
    Uart_TxState_Machine.state = FAN_COM_TEST_SEND_DATA_STATE;
  }
  break;
  case FAN_COM_TEST_SEND_DATA_STATE: {
    if (Uart_TxState_Machine.cnt < Uart_TxState_Machine.length)
    {
      Uart_TxState_Machine.checksum += Uart_TxState_Machine.buffer[Uart_TxState_Machine.cnt];
      (void)Sdk_writeUartU8(Uart_TxState_Machine.buffer[Uart_TxState_Machine.cnt]);
      Uart_TxState_Machine.cnt++;
    }
    else
    {
      (void)Sdk_writeUartU8(Uart_TxState_Machine.checksum >> 8u);
      Uart_TxState_Machine.state = FAN_COM_TEST_SEND_LASTCHK_STATE;
    }
  }
  break;
  case FAN_COM_TEST_SEND_LASTCHK_STATE: {
    (void)Sdk_writeUartU8(Uart_TxState_Machine.checksum);
    Uart_TxState_Machine.state = FAN_COM_TEST_SEND_COMPLETE_STATE;
  }
  break;
  case FAN_COM_TEST_SEND_COMPLETE_STATE:
    Uart_TxState_Machine.state = FAN_COM_TEST_SEND_IDLE_STATE;
    break;
  default: /* Not use */
    break;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_startFrameTx
* Description:
*		Call this function to start data transmition.
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
void FanComTest_startFrameTx(u8 FrameId)
{
  Uart_TxState_Machine.state = FAN_COM_TEST_SEND_ID_STATE;
  Uart_TxState_Machine.id = FrameId;
  Uart_TxState_Machine.cnt = 0u;
  Uart_TxState_Machine.checksum = 0u;
  Uart_TxState_Machine.length = FAN_COM_TEST_DATA_BUF_LEN;
  Common_cpMemory(Uart_TxState_Machine.buffer, uart_sig_ttbl, Uart_TxState_Machine.length);
  (void)Sdk_writeUartU8(Uart_TxState_Machine.id);
}
