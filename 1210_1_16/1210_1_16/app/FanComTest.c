/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:FanComTest.c
********************************************************************
* Project/Product:LXT9818_PMP
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

/* Includes*/
/******************************************************************/
#include "FanComTest.h"
#include "Common.h"
#include "SDK.h"

/* Defines*/
/******************************************************************/
#define DATA_BUF_LEN             8u

#define RECEIVE_ID_STATE         0u
#define CHECK_ID_STATE           1u
#define SEND_DATA_STATE          2u
#define RECEIVE_DATA_STATE       3u
#define SEND_COMPLETE_STATE      4u
#define CHECK_CHKSUM_STATE       5u
#define SEND_LASTCHK_STATE       6u
#define SEND_ID_STATE            7u
#define SEND_IDLE_STATE          9u

#define TIMEOUT_CNT_EN           1u
#define TIMEOUT_CNT_DIS          0u

typedef enum{
	IDErr,
	IDCheckErr,
	ChecksumErr,
	TimeoutErr
}_uart_errtype;

typedef struct{
	u8 id;
	u8 state;
  u8 length;
  u8 cnt;
	u8 buffer[DATA_BUF_LEN];
	u8 chksumbuffer[2u];
	u16 checksum;
	u8 timeoutcnt;
	u8 timeouten;
}_uart_state_machine;

/* Variables */
/******************************************************************/
static _uart_state_machine Uart_RxState_Machine;
static _uart_state_machine Uart_TxState_Machine;

static u8 uart_sig_ttbl[DATA_BUF_LEN];
static u8 uart_sig_rtbl[DATA_BUF_LEN];
static u8 uart_sig_id;

/* Function declaration */
/******************************************************************/
static void FanComTest_UartErrHandle(_uart_errtype errtype);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_WriteU8
* Description:
*		Write data to send buffer.Call FanComTest_TxHandler() to start trans-
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
void FanComTest_WriteU8(u8 signum,u8 value)
{
  if(signum < (DATA_BUF_LEN))
  {
		uart_sig_ttbl[signum] = value;
  }
	else
	{

	}
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_ReadU8
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
u8 FanComTest_ReadU8(u8 signum)
{
	u8 ret = 0u;
  if(signum < (DATA_BUF_LEN))
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
void FanComTest_ResetU8(u8 signum)
{
  if(signum < (DATA_BUF_LEN))
  {
		uart_sig_rtbl[signum] = 0x00u;
  }
	else
	{

	}
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_WriteU16
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
void FanComTest_WriteU16(u8 signum,u16 value)
{
  signum = (signum<<1u) + 1u;
  if(signum < (DATA_BUF_LEN))
  {
		uart_sig_ttbl[signum - 1u] = (value + 32768u)>>8u;
		/*消除polyspace警告,2021-09-22-by zyg*/
		uart_sig_ttbl[signum] = (value + 32768u)&U8_MAX;
  }
	else
	{

	}
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_WriteS16
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
void FanComTest_WriteS16(u8 signum,s16 value)
{
	u16 tmp = 0u;
	
  signum = (signum<<1u) + 1u;
  if(signum < (DATA_BUF_LEN))
  {	
		/*polyspace-begin  MISRA2012:10.8*/
		tmp = (u16)((s32)value + 32768);
		/*polyspace-end  MISRA2012:10.8*/
		uart_sig_ttbl[signum - 1u] = tmp>>8u;
		uart_sig_ttbl[signum] = tmp;
  }
	else
	{

	}
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_ReadU16
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
u16 FanComTest_ReadU16(u8 signum)
{
	u16 ret = U16_MAX;
	
  signum = (signum<<1u) + 2u;
	/*消除polyspace警告,2022-03-09-by zyg*/
	if((signum < DATA_BUF_LEN) && (signum >= 1))
	{
		ret = (((u16)uart_sig_rtbl[signum - 1u])<<8u) | (u16)uart_sig_rtbl[signum];
	}
	
	return ret;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_GetRxID
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
u8 FanComTest_GetRxID(void)
{
	return uart_sig_id;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_UartErrHandle
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
static void FanComTest_UartErrHandle(_uart_errtype errtype)
{
	Uart_RxState_Machine.timeouten = TIMEOUT_CNT_DIS;
	Uart_RxState_Machine.timeoutcnt = 0u;
	Uart_RxState_Machine.state = RECEIVE_ID_STATE;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_RxHandler
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
void FanComTest_RxHandler(void)
{
	u8 temp;
	temp = SDK_UartReadU8();
	
  Uart_RxState_Machine.timeouten = TIMEOUT_CNT_EN;
	Uart_RxState_Machine.timeoutcnt = 0u;
	
	switch(Uart_RxState_Machine.state)
	{
		case RECEIVE_ID_STATE:
		{
			switch(temp)
			{
				case RX_ID_UART_DEBUG:
				case TX_ID_UART_DEBUG:
				case RX_ID_ENTRY_FCT_TEST_MODE:
				case RX_ID_TEST_MOS:
				case RX_ID_TEST_PWM_OUT:	/* No need "break" for above cases. */
					Uart_RxState_Machine.id = temp;
					Uart_RxState_Machine.state = CHECK_ID_STATE;
				break;
				default:
					FanComTest_UartErrHandle(IDErr);
				break;
			}
		}
		break;
		case CHECK_ID_STATE:
		{
			u8 antitemp;
			antitemp = (~temp);
			if(Uart_RxState_Machine.id == antitemp)
			{
				Uart_RxState_Machine.state = RECEIVE_DATA_STATE;
				Uart_RxState_Machine.length = DATA_BUF_LEN;
				Uart_RxState_Machine.cnt = 0u;
				Uart_RxState_Machine.checksum = 0u;
			}
			else
			{
				FanComTest_UartErrHandle(IDCheckErr);
			}
		}
		break;
		case RECEIVE_DATA_STATE:
		{
			Uart_RxState_Machine.buffer[Uart_RxState_Machine.cnt] = temp;
			Uart_RxState_Machine.checksum += temp;
			Uart_RxState_Machine.cnt++;
			if(Uart_RxState_Machine.cnt >= Uart_RxState_Machine.length)
			{
				Uart_RxState_Machine.state = CHECK_CHKSUM_STATE;
				Uart_RxState_Machine.cnt = 0u;
			}
			else
			{

			}
		}
		break;
		case CHECK_CHKSUM_STATE:
		{
			Uart_RxState_Machine.chksumbuffer[Uart_RxState_Machine.cnt] = temp;
			Uart_RxState_Machine.cnt++;
			if(Uart_RxState_Machine.cnt >= 2u)
			{
				u16 chksum = 0u;
				chksum = ((u16)Uart_RxState_Machine.chksumbuffer[0u]<<8u)|Uart_RxState_Machine.chksumbuffer[1u];
				if(chksum == Uart_RxState_Machine.checksum)
				{
					uart_sig_id = Uart_RxState_Machine.id;
					Common_CopyMem(uart_sig_rtbl,Uart_RxState_Machine.buffer,Uart_RxState_Machine.length);
					Uart_RxState_Machine.state = RECEIVE_ID_STATE;
					Uart_RxState_Machine.timeouten = TIMEOUT_CNT_DIS;
					Uart_RxState_Machine.timeoutcnt = 0u;
				}
				else
				{
					FanComTest_UartErrHandle(ChecksumErr);
				}
			}
			else
			{

			}
		}
		break;
		default:/*加注释消除polyspace警告,2021-09-22-by zyg*/
		break;
	}
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_TxHandler
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
void FanComTest_TxHandler(void)
{
	switch (Uart_TxState_Machine.state)
	{
		case SEND_ID_STATE:
		{
			u8 temp;
			temp = Uart_TxState_Machine.id;
			SDK_UartWriteU8(~temp);
			Uart_TxState_Machine.state = SEND_DATA_STATE;
		}
		break;
		case SEND_DATA_STATE:
		{
			if(Uart_TxState_Machine.cnt < Uart_TxState_Machine.length)
			{
				Uart_TxState_Machine.checksum += Uart_TxState_Machine.buffer[Uart_TxState_Machine.cnt];
				SDK_UartWriteU8(Uart_TxState_Machine.buffer[Uart_TxState_Machine.cnt]);
				Uart_TxState_Machine.cnt++;
			}
			else
			{
				SDK_UartWriteU8(Uart_TxState_Machine.checksum>>8u);
				Uart_TxState_Machine.state = SEND_LASTCHK_STATE;
			}
		}
		break;
		case SEND_LASTCHK_STATE:
		{
			SDK_UartWriteU8(Uart_TxState_Machine.checksum);
			Uart_TxState_Machine.state = SEND_COMPLETE_STATE;
		}
		break;
		case SEND_COMPLETE_STATE:
			Uart_TxState_Machine.state = SEND_IDLE_STATE;
		break;
		default:/*加注释消除polyspace警告,2021-09-22-by zyg*/
		break;
	}
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FanComTest_StartFrameTransmission
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
void FanComTest_StartFrameTransmission(u8 FrameId)
{
	Uart_TxState_Machine.state = SEND_ID_STATE;
	Uart_TxState_Machine.id = FrameId;
	Uart_TxState_Machine.cnt = 0u;
	Uart_TxState_Machine.checksum = 0u;
	Uart_TxState_Machine.length = DATA_BUF_LEN;
	Common_CopyMem(Uart_TxState_Machine.buffer,uart_sig_ttbl,Uart_TxState_Machine.length);
	SDK_UartWriteU8(Uart_TxState_Machine.id);
}
