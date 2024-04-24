/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup SCI_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      bsp/SCI/lin_lld_sci.c
*
* @author    FPT Software
*
* @brief     SCI for LIN network
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20101027   v1.0  First version
 * 20111005   v1.1  Updated hardware support, multi timers
 *
 *****************************************************************************/

#include "lin_lld_sci.h"
#include "lin_hw_cfg.h"
#include "lin_common_proto.h"
#include "lin_lld_timesrv.h"
#include "lin_cfg.h"

#if (LIN_MODE == _SLAVE_MODE_)

/***** Globle variable data *****/
/*信号错误计数阈值,22.05.13 by zyg*/
#define LIN_ERROR_CNT_MAX (80u)
/**
* @var static l_u8          ifc
*/
static l_u8          ifc = 0xFF;

/**
* @var static l_u8          state
*/
static l_u8          state = UNINIT;

/**
* @var static lin_status    l_status
*/
static lin_status    l_status;

/**
* @var static l_u8          cnt_byte
*/
static l_u8          cnt_byte = 0;

/**
* @var static l_u8         *ptr
*/
static l_u8          *ptr = 0;

/**
* @var static l_u8          current_id
*/
static l_u8          current_id = 0x00;

/**
* @var static l_u8         *response_buffer
*/
static l_u8          *response_buffer = 0;

/**
* @var static l_u8          pid
*/
static l_u8          pid = 0x80;

/**
* @var static l_u16          tbit
*/
//static l_u16         tbit = 0;

/**
* @var static l_u16          frame_timeout_cnt
*/
static l_u16         frame_timeout_cnt = 0;

/**
* @var static l_u16          res_frame_timeout_cnt
*/
static l_u16         res_frame_timeout_cnt = 0;

/**
* @var static l_u16          idle_timeout_cnt
*/
static l_u16         idle_timeout_cnt = 0;
static l_u8 lastbyte_tx=0;

l_u8 IDEA_TimeOut = 0;
l_u8 Lin_Error_Flag = 0;
static l_u8 Lin_Error_Cnt = 0;

extern const l_u16 lin_max_frame_res_timeout_val[8];
extern l_u8 lin_lld_response_buffer[10];

void lin_lld_sci_init
  (
    /* [IN] LIN interface name*/
    l_ifc_handle iii
  )
{
  /* Config */
  ifc=(l_u8)iii;
  ifc = ifc;
  response_buffer=lin_lld_response_buffer;
  /* Set SCI is Master or Slave */

  /* Initialize SCI */
  /* Set Baudrate Generator and enable */
  /* Set Baudrate.Do this in IFXConfigwazard */
  /* Baudrate Generator run */
  SCU->BCON1.bit.R =     1u;
  /* Set Limits for Baudrate detection. fsys=24M,detect 20k */
  SCU->LINST.bit.BGSEL = 1u;
  /* Sync Err and End detect enable */
  SCU->LINST.bit.SYNEN = 1u;
  /* Enable Brk and Sync detect.Toggle this bit. */
  SCU->LINST.bit.BRDIS = 1u;
  SCU->LINST.bit.BRDIS = 0u;
  /* Clear Flag */
  SCU->LINSCLR.bit.BRKC = 1u;
  SCU->LINSCLR.bit.EOFSYNC = 1u;
  SCU->LINSCLR.bit.ERRSYNC = 1u;
  /* Enable UART reception */
  UART1->SCON.bit.REN =   1u;
  SCU->MODIEN1.bit.RIEN1 = 1u;
  /* Enable hardware and interrupt.Do this in IFXConfigwazard */

  /* Enter IDLE state */
  lin_goto_idle_state();
} /* End of function lin_lld_sci_init( l_ifc_handle iii ) */


void lin_lld_sci_deinit
  (
  )
{
  state=UNINIT;
  lin_lld_sci_int_disable();
}


void lin_lld_sci_tx_wake_up
  (
  )
{
  if((state == IDLE) || (state == SLEEP_MODE)) {

    /* Send wake signal byte=0x80 */
    SCI_WriteData(SCIDRH_R8_MASK);
    /* Set Lin state to idle */
    lin_goto_idle_state();
  }
} /* End function lin_lld_sci_tx_wake_up() */


void lin_lld_sci_int_enable
  (
  )
{
/*--------------------------------------------------------------------*/

  /* Can't enable in interrupt context */
  if((state == PROC_CALLBACK) || (state==UNINIT) || (state==SLEEP_MODE)) return;
  /* Enable SCI Channel.Enable Receiver and receive intterupt. */
  SCI_REn(true);
  SCI_RIEn(true);
  /* Enable Timeout */
} /* End function lin_lld_sci_int_enable() */


void lin_lld_sci_int_disable
  (
  )
{
/*--------------------------------------------------------------------*/
  /* Can't disable in interrupt context */
  if((state == PROC_CALLBACK) || (state==UNINIT) || (state==SLEEP_MODE)) return;

  while(state != IDLE) { }
  /* Disable SCI Channel*/
  SCI_REn(false);
  SCI_RIEn(false);
} /* End function lin_lld_sci_int_disable() */


void lin_lld_sci_ignore_response
  (
  )
{
  lin_goto_idle_state();
}


void lin_lld_sci_set_low_power_mode
  (
  )
{
  /* Configure Hw code */
  SCI_HwSetLowPower();
  /* Set Lin status = receiving data*/
  state=SLEEP_MODE;
}


void lin_lld_sci_rx_response
  (
    /* [IN] Length of response data expect to wait */
    l_u8  msg_length
  )
{
  /* Put response length and pointer of response buffer into descriptor */
  *(response_buffer)=msg_length;
  cnt_byte=0;
  ptr=response_buffer;
  /* Set Lin status = receiving data*/
  state=RECV_DATA;
}


void lin_lld_sci_rx
  (
  )
{
  /* Not yet implemented */
}


void lin_lld_sci_tx
  (
  )
{
  /* Not yet implemented */
}


void lin_lld_sci_aux
  (
  )
{
  /* Not yet implemented */
}


void lin_lld_sci_tx_response
  (
  )
{
/*--------------------------------------------------------------------*/
  /* calculate checksum */
  response_buffer[*(response_buffer)+1] = lin_checksum(response_buffer, pid );
  cnt_byte=1;
  /* Send First byte */
  SCI_WriteData(response_buffer[1]);
  /* Set LIN Status */
  state = SEND_DATA;
} /* End function lin_lld_sci_tx_response() */


l_u8 lin_lld_sci_get_status
  (
  )
{
  return l_status.byte;
}


l_u8 lin_lld_sci_get_state()
{
  return state;
}

l_u16 lin_lld_sci_get_idle_time(void)
{
  return (_MAX_IDLE_TIMEOUT_ - idle_timeout_cnt);
}

void lin_lld_sci_timeout
  (
  )
{
/* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  if (LD_CHECK_N_CR_TIMEOUT == tl_check_timeout_type){
    if(0 == --tl_check_timeout){
      /* update status of transport layer */
      tl_service_status = LD_SERVICE_ERROR;
      tl_receive_msg_status = LD_N_CR_TIMEOUT;
      tl_rx_msg_status = LD_N_CR_TIMEOUT;
      tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
      tl_diag_state = LD_DIAG_IDLE;
    }
  }

  if (LD_CHECK_N_AS_TIMEOUT == tl_check_timeout_type){
    if(0 == --tl_check_timeout){
      /* update status of transport layer */
      tl_service_status = LD_SERVICE_ERROR;
      tl_tx_msg_status = LD_N_AS_TIMEOUT;
      tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
      tl_diag_state = LD_DIAG_IDLE;
    }
  }
#endif /* END (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  */

  switch(state) {
    case IDLE:
      if(idle_timeout_cnt==0) {
        /* Set LIN mode to sleep mode */
        //lin_goto_sleep_flg = 1;
        /* trigger callback */
        //CALLBACK_HANDLER(ifc, LIN_LLD_BUS_ACTIVITY_TIMEOUT, 0xFF);
        /* goback to IDLE, reset max idle timeout */
        //idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
        /* Set state to sleep mode */
        //state = SLEEP_MODE;
				IDEA_TimeOut = 1;
      } 
			else 
			{
				idle_timeout_cnt--;
				IDEA_TimeOut = 0;
			}
      break;
    case SEND_PID:    /* Master */
    case RECV_SYN:
    case RECV_PID:
    case SEND_DATA:
    case SEND_DATA_COMPLETED:
      /* timeout send has occurred - change state of the node and inform core */
      if(0 == frame_timeout_cnt) {
        lin_goto_idle_state();
      }
      else frame_timeout_cnt--;
      break;
    case RECV_DATA:
      /* timeout receive has occurred - change state of the node and inform core */
      if(0 == res_frame_timeout_cnt) {
        if(cnt_byte > 0) {
					/*发生错误计数增加,22.05.13 by zyg*/
					if(Lin_Error_Cnt < LIN_ERROR_CNT_MAX)
					{
						Lin_Error_Cnt++;
					}
					else
					{
						Lin_Error_Flag = 1;
					}
					
          /* set lin status: error_in_response */
          l_status.byte |= LIN_STA_ERROR_RESP;
					/* trigger callback */
					CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_NODATA_TIMEOUT, current_id);
        }
        lin_goto_idle_state();
      } else res_frame_timeout_cnt--;
      break;
    case PROC_CALLBACK:
      break;
    default:
    ;
  }
} /* End function lin_lld_sci_timeout() */


/*** INTERNAL FUNTIONS ***/

void lin_goto_idle_state
  (
  )
{
  /* set lin status: ~bus_activity */
  l_status.byte &= ~LIN_STA_BUS_ACTIVITY;
  /* Set max idle timeout */
  idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
  SCI_BreakDetect_En();
  state=IDLE;
} /* End function lin_goto_idle_state() */


void lin_lld_sci_lastbyte_tx_set(l_u8 byte)
{
    lastbyte_tx=byte;
}

l_u8 lin_lld_sci_lastbyte_tx_get(void)
{
    return lastbyte_tx;
}

/*
9873@40MHz
Brek:6.2us
Sync:4*4.8us
SyncRI:6.6us
PID:70.4us
Data:8*8.0us
ChkSum:20.4us

*/
void lin_lld_sci_isr
  (
  )
{
  l_u8 tmp_byte;

  //SCI_RIClr();

  if(SCI_Sync() != 0) //Sync byte received
  {
    SCI_BrkClr();
    SCI_SyncClr();

    /* reset lin status */
    l_status.byte = LIN_STA_BUS_ACTIVITY;
    /* Set max frame timeout */
    frame_timeout_cnt = lin_max_frame_res_timeout_val[7];

    state=RECV_SYN;
  }
  else if(SCI_SyncErr()!=0) //Sync byte error
  {
    SCI_BrkClr();
    SCI_SyncErrClr();

#if (LIN_PROTOCOL == PROTOCOL_J2602)
    CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR,current_id);
#endif /* End of (LIN_PROTOCOL == PROTOCOL_J2602) */
    lin_goto_idle_state();
   }
  else //BIT ERROR DETECTED
  {
    if(SCI_TI() && SCI_RI() && SCI_ByteErrDetect()
    && ((state == SEND_DATA) || (state == SEND_DATA_COMPLETED))
    )
    {
      SCI_ByteErrClear();
      SCI_TIClr();
      SCI_RIClr();
			/*发生错误计数增加,22.05.13 by zyg*/
			if(Lin_Error_Cnt < LIN_ERROR_CNT_MAX)
			{
				Lin_Error_Cnt++;
			}
			else
			{
				Lin_Error_Flag = 1;
			}
      /* set lin status: error_in_response, readback_error */
      l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_READBACK_ERR);
      if((state == SEND_DATA) || (state == SEND_DATA_COMPLETED))
        /* trigger callback */
        CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR, current_id);
      lin_goto_idle_state();
      return;
    }
  }

  if(SCI_FrameErrDetect() && (state != SLEEP_MODE) && (state != IDLE)) //FRAME ERROR DETECTED
  {
    /* Clear FE flags */
    SCI_FrameErrClr();
		/*发生错误计数增加,22.05.13 by zyg*/
		if(Lin_Error_Cnt < LIN_ERROR_CNT_MAX)
		{
			Lin_Error_Cnt++;
		}
		else
		{
			Lin_Error_Flag = 1;
		}
    /* set lin status: error_in_response, framing_error */
    l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_FRAME_ERR);
    /* trigger callback */
    if((state == RECV_DATA)||(state == SEND_DATA) || (state == SEND_DATA_COMPLETED))
    {
      CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_FRAME_ERR, current_id);
    }
    lin_goto_idle_state();

    SCI_RIClr();
  }
  else //BYTE RECIEVED
  {
    SCI_RIClr();
    SCI_ReadData(tmp_byte);

    switch(state)
    {
      case RECV_SYN:
        if(tmp_byte == 0x55u)
        {
          state=RECV_PID;
        }
        else
        {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
          CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR, current_id);
#endif /* End of (LIN_PROTOCOL == PROTOCOL_J2602) */
          lin_goto_idle_state();
        }
        break;
      case RECV_PID:
        /* checkparity and extrait PID */
        pid=tmp_byte;

        current_id = lin_process_parity(tmp_byte,CHECK_PARITY);

        if(current_id !=0xFF) {
          /*****************************************/
          /*** ID received correctly - parity OK ***/
          /*****************************************/
          /* trigger callback */
          CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_PID_OK, current_id);
          /* Set Maximum response frame timeout */
          res_frame_timeout_cnt = lin_max_frame_res_timeout_val[*(response_buffer) - 1];
        }
        else {
          /*****************************************/
          /*** ID Parity Error                   ***/
          /*****************************************/
					/*发生错误计数增加,22.05.13 by zyg*/
					if(Lin_Error_Cnt < LIN_ERROR_CNT_MAX)
					{
						Lin_Error_Cnt++;
					}
					else
					{
						Lin_Error_Flag = 1;
					}
          /* set lin status: parity_error */
          l_status.byte |= LIN_STA_PARITY_ERR;
          /* trigger callback */
          CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_PID_ERR, 0xFF);
          lin_goto_idle_state();
        }

        break;
      /******************************
      *** 4.4 SLAVE: Receiving data //365us
      *******************************/
      case RECV_DATA:
        ptr++;
        *(ptr)=tmp_byte;      /* address of ptr:0x3AB3 */
        /* Check bytes received fully */
        if(cnt_byte==(response_buffer[0])) {
          /* checksum checking */
          if(lin_checksum(response_buffer, pid)==tmp_byte) {
            /*******************************************/
            /***  RX Buffer Full - Checksum OK       ***/
            /*******************************************/
						if(Lin_Error_Cnt > 0)
						{
							Lin_Error_Cnt--;
						}
						else{
							Lin_Error_Flag = 0;
						}
            /* set lin status: successful_transfer */
            l_status.byte |= LIN_STA_SUCC_TRANSFER;
            /* disable RX interrupt */
            SCI_REn(false);
            SCI_RIEn(false);
            state=PROC_CALLBACK;
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_RX_COMPLETED, current_id);
            /* enable RX interrupt */
            SCI_REn(true);
            SCI_RIEn(true);
            if (SLEEP_MODE != state){
              lin_goto_idle_state();
            }
          }
          else
          {
						/*发生错误计数增加,22.05.13 by zyg*/
						if(Lin_Error_Cnt < LIN_ERROR_CNT_MAX)
						{
							Lin_Error_Cnt++;
						}
						else
						{
							Lin_Error_Flag = 1;
						}
            /*******************************************/
            /***  RX Buffer Full - Checksum ERROR    ***/
            /*******************************************/
            /* set lin status: error_in_response, checksum_error */
            l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_CHECKSUM_ERR);
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_CHECKSUM_ERR, current_id);
            lin_goto_idle_state();
          }
        }
        cnt_byte++;

        break;
      /******************************
      *** 4.5 SLAVE: Sending data //2.5us
      *******************************/
      case SEND_DATA:
        if (0 == (SCI_TI())) {
          SCI_TIClr();
          CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR, current_id);
          state = PROC_CALLBACK;
          lin_goto_idle_state();
          break;
        }

        if(cnt_byte==(response_buffer[0])) {
          /* Send checksum byte */
          SCI_WriteData(response_buffer[cnt_byte+1]);
          state= SEND_DATA_COMPLETED;
        } else {
          /* Send byte next */
          cnt_byte++;
          SCI_WriteData(response_buffer[cnt_byte]);
        }

        break;
      /******************************
      *** 4.7 SLAVE: Sending data compeleted  //35us
      *******************************/
      case SEND_DATA_COMPLETED:
        /*******************************************/
        /***  TX Buffer Empty - Checksum Sent    ***/
        /*******************************************/
        if (0 == (SCI_TI() )) {
          SCI_TIClr();
          CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR, current_id);
          state = PROC_CALLBACK;
          lin_goto_idle_state();
          break;
        }
        /* set lin status: successful_transfer */
        l_status.byte |= LIN_STA_SUCC_TRANSFER;
        /* disable RX interrupt */
        SCI_REn(false);
        SCI_RIEn(false);
        state=PROC_CALLBACK;
        /* trigger callback */
        CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_TX_COMPLETED, current_id);
        SCI_REn(true);
        SCI_RIEn(true);
        /* enable RX interrupt */
        lin_goto_idle_state();
        SCI_TIClr();
        break;
       /******************************
      *** 4.8 SLAVE: Low power mode
      *******************************/
      case SLEEP_MODE:
        if ((tmp_byte == 0xF0) || (tmp_byte == 0xE0) || (tmp_byte == 0xC0) || (tmp_byte == 0x80) || (tmp_byte == 0x00)) 
        {
          /* Set idle timeout again */
          lin_goto_idle_state();
        }

        break;
      default:
      ;
    }
  }
} /* End function lin_lld_sci_isr() */

#endif /* End (LIN_MODE == _SLAVE_MODE_) */

/* Unmodifed under section fo TLE9842QX */
#if (LIN_MODE == _MASTER_MODE_)
/***** Globle variable data *****/

extern lin_node lin_node_descrs[NUM_OF_SCI_CHANNEL];
extern const l_u16 lin_max_frame_res_timeout_val[LIN_NUM_OF_IFCS][8];

#ifdef MULTI_TIMER_MODE
extern const l_u16 max_idle_timeout[LIN_NUM_OF_IFCS];
#endif /* End  MULTI_TIMER_MODE */
/***** LOW-LEVEL API *****/


void lin_lld_sci_init
  (
    /* [IN] sci channel name */
    sci_channel_name  channel,
    /* [IN] LIN interface name*/
    l_ifc_handle iii
  )
{

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V5)
  l_u16 tmp;
  lin_node *lnode_p;
  tSCI* _pSCI;
  lin_configuration *lconf_p;
#if defined(_MC9S12VR48_H) || defined(_MC9S12VR64_H)//def _MC9S12VR64_H    /*** modify by me ***/
#if (_SCI0_)
  /* Set up for LIN PHY */
  LPCR_LPE = 1; /* Enable LIN PHY*/
  LPSLRM = 0x01; /* Slew Rate for 9600 baudrate */  /*** modify by me ***/
#endif /* END SCI0 */
#endif /* END ifdef _MC9S12VR64_H */
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  _pSCI = lnode_p->pSCI;
  lconf_p = (lin_configuration *)&lin_ifc_configuration[iii];

  /* Config */
  lnode_p->ifc=(l_u8)iii;
  lnode_p->response_buffer=lconf_p->response_buffer;
  /* Set SCI is Master or Slave */
  lnode_p->func=(l_bool)lconf_p->function;
  lnode_p->tbit=(l_u16)(1000000/lconf_p->baud_rate);

  /* Initialize SCI */
  /* Set baud rate */
  _pSCI->scisr2.byte = 0x00;
  tmp = (l_u16)(MCU_BUS_FREQ/lconf_p->baud_rate/16);
  _pSCI->scibdh.byte  = (l_u8)(tmp>>8);
  _pSCI->scibdl.byte  = tmp&0xff;

  /* Disable SCI in wait mode and enable bit count after stop bit */
  _pSCI->scicr1.byte = (SCICR1_ILT_MASK | SCICR1_SCISWAI_MASK);
  /* Enable use of 13bit breaks and SCI frame for LIN */
  _pSCI->scisr2.byte  = (SCISR2_AMAP_MASK | SCISR2_BRK13_MASK);
  /* Enable mismatch error detection */
  _pSCI->sciasr1.byte = SCIASR1_BERRIF_MASK;
  /* Enable bit error interrupt and break detect flags */
  _pSCI->sciacr1.byte = (SCIACR1_BERRIE_MASK | SCIACR1_BKDIE_MASK);
  /* Enable break detect circuit and timing bit error detection at 9th time stick */
  _pSCI->sciacr2.byte = (SCIACR2_BERRM0_MASK | SCIACR2_BKDFE_MASK);
  /* Enable tranceiver interrupt */
  _pSCI->scicr2.byte = (SCICR2_TE_MASK | SCICR2_RE_MASK | SCICR2_RIE_MASK);

  /* Enter IDLE state */
  lin_goto_idle_state(channel);
#endif /* End (SCI_VERSION != SCI_V5) */
} /* End function lin_lld_sci_init(sci_channel_name  channel, l_ifc_handle iii) */


void lin_lld_sci_deinit
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  lin_node *lnode_p;
  lnode_p->state=UNINIT;
  lin_lld_sci_int_disable(channel);

  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  lnode_p->state=UNINIT;
  lin_lld_sci_int_disable(channel);
} /* End function lin_lld_sci_deinit(sci_channel_name  channel) */


void lin_lld_sci_tx_header
  (
    /* [IN] sci channel name */
    sci_channel_name  channel,
    /* [IN] PID to be send */
    l_u8  pid_id
  )
{
  lin_node *lnode_p;

  lnode_p = (lin_node *)&lin_node_descrs[channel];
  /* Check the SCI is Master ? */
  if(lnode_p->func) return;
  /* Make PID and put PID into the ongoing buffer */
  lnode_p->current_id = pid_id;
  lnode_p->pid = lin_process_parity(pid_id, MAKE_PARITY);
  /* Set LIN Status */
  lnode_p->state = SEND_BREAK;
  /* Send Break*/
  lnode_p->pSCI->scicr2.byte |= SCICR2_SBK_MASK;
  lnode_p->pSCI->scicr2.byte &= ~SCICR2_SBK_MASK;
} /* End function lin_lld_sci_tx_header(sci_channel_name  channel, l_u8  pid_id) */


void lin_lld_sci_tx_wake_up
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
#if (SCI_VERSION == SCI_V2)
  lin_node *lnode_p;
  lnode_p = &lin_node_descrs[channel];
  if((lnode_p->state == IDLE) || (lnode_p->state == SLEEP_MODE)) {
  /* Send wake signal byte=0x80 */
    lnode_p->pSCI->scid.byte = 0x80;

    /* Set Lin state to idle */
    lin_goto_idle_state(channel);
  }
#endif /* End (SCI_VERSION == SCI_V2) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V4)
  lin_node *lnode_p;
  l_u8 sci_flag_sr1;
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  if((lnode_p->state == IDLE) || (lnode_p->state == SLEEP_MODE)) {
    sci_flag_sr1 = lnode_p->pSCI->scisr1.byte;
    /* Send wake signal byte=0x80 */
    lnode_p->pSCI->scid.byte = SCID_R7_T7_MASK;
    /* Set Lin state to idle */
    lin_goto_idle_state(channel);
  }
#endif /* End (SCI_VERSION == SCI_V4) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V5)
  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  if((lnode_p->state == IDLE) || (lnode_p->state == SLEEP_MODE)) {
    /* Send wake signal byte=0x80 */
    lnode_p->pSCI->scidrl.byte = SCIDRH_R8_MASK;
    /* Set Lin state to idle */
    lin_goto_idle_state(channel);
  }
#endif /* End (SCI_VERSION == SCI_V5) */
} /* End function lin_lld_sci_tx_wake_up(sci_channel_name  channel) */


void lin_lld_sci_int_enable
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  lin_node *lnode_p;
  #if (SCI_VERSION == SCI_V2)
  lnode_p = &lin_node_descrs[channel];
  #else
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  #endif /* End (SCI_VERSION == SCI_V2) */

  /* Can't enable in interrupt context */
  if((lnode_p->state == PROC_CALLBACK) || (lnode_p->state==UNINIT) || (lnode_p->state==SLEEP_MODE)) return;

  /* Enable SCI Channel*/
  lnode_p->pSCI->scicr2.byte |= (SCICR2_RE_MASK | SCICR2_RIE_MASK);
}


void lin_lld_sci_int_disable
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  lin_node *lnode_p;
  #if (SCI_VERSION == SCI_V2)
  lnode_p = &lin_node_descrs[channel];
  #else
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  #endif /* End (SCI_VERSION == SCI_V2) */

  /* Can't disable in interrupt context */
  if((lnode_p->state == PROC_CALLBACK) || (lnode_p->state==UNINIT) || (lnode_p->state==SLEEP_MODE)) return;

  while(lnode_p->state != IDLE) { }
  /* Disable SCI Channel*/
  lnode_p->pSCI->scicr2.byte &= ~(SCICR2_RE_MASK | SCICR2_RIE_MASK);
}


void lin_lld_sci_ignore_response
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  lin_goto_idle_state(channel);
}


void lin_lld_sci_set_low_power_mode
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  lin_node *lnode_p;

  /* Get Lin node descriptor */
  lnode_p = (lin_node *)&lin_node_descrs[channel];

  /* Configure Hw code */

  /* Set Lin status = receiving data*/
  lnode_p->state=SLEEP_MODE;
}


void lin_lld_sci_rx_response
  (
    /* [IN] sci channel name */
    sci_channel_name  channel,
    /* [IN] Length of response data expect to wait */
    l_u8  msg_length
  )
{
  lin_node *lnode_p;

  /* Get Lin node descriptor */
  #if (SCI_VERSION == SCI_V2)
  lnode_p = &lin_node_descrs[channel];
  #else
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  #endif /* End (SCI_VERSION == SCI_V2) */

  /* Put response length and pointer of response buffer into descriptor */
  *(lnode_p->response_buffer)=msg_length;
  lnode_p->cnt_byte=0;
  lnode_p->ptr=lnode_p->response_buffer;

  /* Set Lin status = receiving data*/
  lnode_p->state=RECV_DATA;
}


void lin_lld_sci_rx
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  /* Not yet implemented */
}


void lin_lld_sci_tx
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{

  /* Not yet implemented */
}


void lin_lld_sci_aux
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  /* Not yet implemented */
}


void lin_lld_sci_tx_response
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
#if (SCI_VERSION != SCI_V5)
  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];

  /* calculate checksum */
  lnode_p->response_buffer[*(lnode_p->response_buffer)+1] = lin_checksum(lnode_p->response_buffer , lnode_p->pid);
  lnode_p->cnt_byte=1;
  /* Send First byte */
  lnode_p->pSCI->scid.byte = lnode_p->response_buffer[1];
  /* Set LIN Status */
  lnode_p->state = SEND_DATA;

#else

  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];

  /* calculate checksum */
  lnode_p->response_buffer[*(lnode_p->response_buffer)+1] = lin_checksum(lnode_p->response_buffer , lnode_p->pid );
  lnode_p->cnt_byte=1;
  /* Send First byte */
  lnode_p->pSCI->scidrl.byte = lnode_p->response_buffer[1];
  /* Set LIN Status */
  lnode_p->state = SEND_DATA;
#endif /* End (SCI_VERSION != SCI_V5) */
} /* End function lin_lld_sci_tx_response(sci_channel_name  channel) */


l_u8 lin_lld_sci_get_status
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];

  return lnode_p->l_status.byte;
}


l_u8 lin_lld_sci_get_state(sci_channel_name channel)
{
  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];

  return lnode_p->state;
}


void lin_lld_sci_timeout
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  register lin_node *lnode_p;
#ifdef MULTI_TIMER_MODE
   l_u8 i;
#endif /* End MULTI_TIMER_MODE */
/* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
  lin_configuration * conf;
  lin_tl_descriptor *tl_conf;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
  lnode_p = (lin_node *)&lin_node_descrs[channel];
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)

  conf = (lin_configuration *)&lin_ifc_configuration[(l_ifc_handle)lnode_p->ifc];
  /* Get TL configuration */
  tl_conf = conf->tl_desc;

  if (LD_CHECK_N_CR_TIMEOUT == tl_conf->tl_check_timeout_type){
    if(0 == --tl_conf->tl_check_timeout){
      /* switch to normal table */
      if (_MASTER_ == conf->function){
        *(conf->active_schedule_id) = *(conf->previous_schedule_id);
        conf->schedule_start_entry[*(conf->active_schedule_id)] = 0;
      }
      /* update status of transport layer */
      *conf->diagnostic_mode = DIAG_NONE;
      *conf->tl_service_status = LD_SERVICE_ERROR;
      tl_conf->tl_receive_msg_status = LD_N_CR_TIMEOUT;
      tl_conf->tl_rx_msg_status = LD_N_CR_TIMEOUT;
      tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
    }
  }

  if (LD_CHECK_N_AS_TIMEOUT == tl_conf->tl_check_timeout_type){
    if(0 == --tl_conf->tl_check_timeout){
      /* switch to normal table */
      if (_MASTER_ == conf->function){
        *(conf->active_schedule_id) = *(conf->previous_schedule_id);
        conf->schedule_start_entry[*(conf->active_schedule_id)] = 0;
      }
      /* update status of transport layer */
      *conf->diagnostic_mode = DIAG_NONE;
      *conf->tl_service_status = LD_SERVICE_ERROR;
      tl_conf->tl_tx_msg_status = LD_N_AS_TIMEOUT;
      tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
    }
  }
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/
#if (SCI_VERSION == SCI_V2)

  switch(lnode_p->state) {

    case IDLE:
      if(lnode_p->idle_timeout_cnt==0) {
        /* trigger callback */
        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_BUS_ACTIVITY_TIMEOUT, 0xFF);
        /* goback to IDLE, reset max idle timeout */
        lnode_p->idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
      } else lnode_p->idle_timeout_cnt--;

      break;

    case SEND_PID:    /* Master */
    case RECV_SYN:
    case RECV_PID:
    case SEND_DATA:
    case SEND_DATA_COMPLETED:
      /* timeout send has occurred - change state of the node and inform core */
      if(0 == lnode_p->frame_timeout_cnt)
        lin_goto_idle_state(channel);
      else lnode_p->frame_timeout_cnt--;
      break;
    case RECV_DATA:
      /* timeout receive has occurred - change state of the node and inform core */
      if(lnode_p->res_frame_timeout_cnt == 0) {
        if(lnode_p->cnt_byte){
          /* set lin status: error_in_response */
          lnode_p->l_status.byte |= LIN_STA_ERROR_RESP;
    /* trigger callback */
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_NODATA_TIMEOUT, lnode_p->current_id);
        }
        lin_goto_idle_state(channel);
      } else lnode_p->res_frame_timeout_cnt--;

      break;
    case PROC_CALLBACK:
      break;
    default:
    ;
  }
#endif /* End (SCI_VERSION == SCI_V2) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V4)

  switch(lnode_p->state) {

    case IDLE:
      if(lnode_p->idle_timeout_cnt==0) {
        /* trigger callback */
        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_BUS_ACTIVITY_TIMEOUT, 0xFF);
        /* goback to IDLE, reset max idle timeout */
        lnode_p->idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
        /* disable LIN break detect interrupt */
        lnode_p->pSCI->scisr2.bit.lbkde = 0;
      } else lnode_p->idle_timeout_cnt--;

      break;

    case SEND_PID:    /* Master */
    case RECV_SYN:
    case RECV_PID:
    case SEND_DATA:
    case SEND_DATA_COMPLETED:
      /* timeout send has occurred - change state of the node and inform core */
      if(0 == lnode_p->frame_timeout_cnt)
        lin_goto_idle_state(channel);
      else lnode_p->frame_timeout_cnt--;
      break;
    case RECV_DATA:
      /* timeout receive has occurred - change state of the node and inform core */
      if(0 == lnode_p->res_frame_timeout_cnt) {
        if(lnode_p->cnt_byte){
          /* set lin status: error_in_response */
          lnode_p->l_status.byte |= LIN_STA_ERROR_RESP;
        /* trigger callback */
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_NODATA_TIMEOUT, lnode_p->current_id);
        }
        lin_goto_idle_state(channel);
      } else lnode_p->res_frame_timeout_cnt--;

      break;
    case PROC_CALLBACK:
      break;
    default:
      break;
  }
#endif /* End (SCI_VERSION == SCI_V4) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V5)

  switch(lnode_p->state) {

    case IDLE:
      if(lnode_p->idle_timeout_cnt==0) {
        /* trigger callback */
        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_BUS_ACTIVITY_TIMEOUT, 0xFF);
        /* goback to IDLE, reset max idle timeout */
#ifdef MULTI_TIMER_MODE
         for(i=0;i<LIN_NUM_OF_IFCS;i++)
         if (channel == lin_virtual_ifc[i]){
            lnode_p->idle_timeout_cnt=max_idle_timeout[i];
            break;
         }
#else
        lnode_p->idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
#endif /* End (TIMER_MODE == MULTI_TIMER) */
      } else lnode_p->idle_timeout_cnt--;

      break;

    case SEND_PID:    /* Master */
    case RECV_SYN:
    case RECV_PID:
    case SEND_DATA:
    case SEND_DATA_COMPLETED:
      /* timeout send has occurred - change state of the node and inform core */
      if(0 == lnode_p->frame_timeout_cnt) {
        lin_goto_idle_state(channel);
      }
      else lnode_p->frame_timeout_cnt--;
      break;
    case RECV_DATA:
      /* timeout receive has occurred - change state of the node and inform core */
      if(0 == lnode_p->res_frame_timeout_cnt) {
        if(lnode_p->cnt_byte) {
          /* set lin status: error_in_response */
          lnode_p->l_status.byte |= LIN_STA_ERROR_RESP;
        /* trigger callback */
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_NODATA_TIMEOUT, lnode_p->current_id);
        }
        lin_goto_idle_state(channel);
      } else lnode_p->res_frame_timeout_cnt--;

      break;
    case PROC_CALLBACK:
      break;
    default:
    ;
  }
#endif /* End (SCI_VERSION == SCI_V5) */
} /* End function lin_lld_sci_timeout(sci_channel_name  channel) */

/*** INTERNAL FUNTIONS ***/


void lin_goto_idle_state
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
#if (SCI_VERSION == SCI_V2)
  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  /* set lin status: ~bus_activity */
  lnode_p->l_status.byte &= ~LIN_STA_BUS_ACTIVITY;
  /* Set max idle timeout */
  lnode_p->idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
  SCI_BreakDetect_En();
  lnode_p->state=IDLE;
#endif /* End (SCI_VERSION == SCI_V2) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V4)
  lin_node *lnode_p;
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  /* set lin status: ~bus_activity */
  lnode_p->l_status.byte &= ~LIN_STA_BUS_ACTIVITY;
  /* Set max idle timeout */
  lnode_p->idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
  lnode_p->state=IDLE;
  /* Enable LBK detect */
  lnode_p->pSCI->scisr2.bit.lbkde = 1;
#endif /* End (SCI_VERSION == SCI_V4) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V5)
  lin_node *lnode_p;
#ifdef MULTI_TIMER_MODE
   l_u8 i;
#endif /* End MULTI_TIMER_MODE */
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  /* set lin status: ~bus_activity */
  lnode_p->l_status.byte &= ~LIN_STA_BUS_ACTIVITY;
  /* Set max idle timeout */
#ifdef MULTI_TIMER_MODE
  for(i=0;i<LIN_NUM_OF_IFCS;i++)
     if (channel == lin_virtual_ifc[i]){
        lnode_p->idle_timeout_cnt=max_idle_timeout[i];
        break;
     }
#else
  lnode_p->idle_timeout_cnt=_MAX_IDLE_TIMEOUT_;
#endif /* End MULTI_TIMER_MODE */
  lnode_p->state=IDLE;
#endif /* End (SCI_VERSION == SCI_V5) */
} /* End function lin_goto_idle_state(sci_channel_name  channel) */

void lin_lld_sci_err_isr
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
#if (SCI_VERSION == SCI_V2)
  l_u8 sci_flag_sr1;
  l_u8 tmp_byte;

  register lin_node *lnode_p;     /* local pointer to the lin node descriptor */
  volatile tSCI *_pSCI;

  lnode_p = (lin_node *)&lin_node_descrs[channel];
  _pSCI = lnode_p->pSCI;

  sci_flag_sr1 = _pSCI->scisr1.byte;
  tmp_byte = _pSCI->scid.byte;
  /******************************
  *** 1. BREAK DETECTED
  *******************************/
  if(0x00 == tmp_byte) {
   /* check state of node is SLEEP_MODE */
   if (SLEEP_MODE == lnode_p->state){
      lin_goto_idle_state(channel);
      return;
    }
    /* reset lin status */
    lnode_p->l_status.byte = LIN_STA_BUS_ACTIVITY;
    /* Set max frame timeout */
    lnode_p->frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][7];
    /******************************
    *** 1.1 MASTER NODE: Sending SYN field
    *******************************/
    if(lnode_p->func == _MASTER_/* Master */) {
      lnode_p->state=SEND_PID;
      /* Send syn field */
      _pSCI->scid.byte = 0x55;
    }
    /******************************
    *** 1.2 SLAVE NODE: Wait for SYN
    *******************************/
    else {
      lnode_p->state=RECV_SYN;
    }

    return;
  }
  /******************************
  *** 3. FRAME ERROR DETECTED
  *******************************/
  else {
    /* set lin status: error_in_response, framing_error */
    lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_FRAME_ERR);
    /* trigger callback */
    if((lnode_p->state == RECV_DATA)||(lnode_p->state == SEND_DATA)||(lnode_p->state == SEND_DATA_COMPLETED))
      CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_FRAME_ERR, lnode_p->current_id);
    lin_goto_idle_state(channel);
  }
#endif /* End (SCI_VERSION = SCI_V2) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V4)
  l_u8 sci_flag_sr1;
  l_u8 tmp_byte;

  register lin_node *lnode_p;     /* local pointer to the lin node descriptor */
  lnode_p = (lin_node *)&lin_node_descrs[channel];

  sci_flag_sr1 = lnode_p->pSCI->scisr1.byte;
  tmp_byte = lnode_p->pSCI->scid.byte;

  /******************************
  *** 3. FRAME ERROR DETECTED
  *******************************/
    /* set lin status: error_in_response, framing_error */
    lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_FRAME_ERR);
    /* trigger callback */
    if((lnode_p->state == RECV_DATA)||(lnode_p->state == SEND_DATA)||(lnode_p->state == SEND_DATA_COMPLETED)) {
      CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_FRAME_ERR, lnode_p->current_id);
    }
    lin_goto_idle_state(channel);
#endif /* End (SCI_VERSION = SCI_V4) */
} /* End function lin_lld_sci_err_isr(sci_channel_name  channel) */

void lin_lld_sci_rx_isr
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
#if (SCI_VERSION == SCI_V2)
  l_u8 sci_flag_sr1;
  l_u8 tmp_byte;

  register lin_node *lnode_p;     /* local pointer to the lin node descriptor */
  volatile tSCI* _pSCI;           /* pointer to the SCI peripheral */

  lnode_p = (lin_node *)&lin_node_descrs[channel];
  _pSCI = lnode_p->pSCI;

  sci_flag_sr1 = _pSCI->scisr1.byte;
  tmp_byte = _pSCI->scid.byte;
  /******************************
  *** 4. BYTE RECIEVED
  *******************************/
  if(0 != (sci_flag_sr1&SCISR1_RDRF_MASK)) {

    switch(lnode_p->state) {
      /******************************
      *** 4.1 MASTER: Sending PID of frame
      *******************************/
      case SEND_PID:
        lnode_p->state=RECV_PID;
        /* Send PID byte */
        _pSCI->scid.byte = lnode_p->pid;
        break;
      /******************************
      *** 4.2 SLAVE: Receiving SYN byte
      *******************************/
      case RECV_SYN:
        if (0x55 == tmp_byte){
          lnode_p->state=RECV_PID;
        } else {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
#endif /* End of (LIN_PROTOCOL == PROTOCOL_J2602) */
          lin_goto_idle_state(channel);
        }
        break;
      /******************************
      *** 4.3 SLAVE: Receiving PID
      *******************************/
      case RECV_PID:
        if(lnode_p->func == _MASTER_) { /*Master*/
          /* trigger callback */
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
          /* Set Maximum response frame timeout */
          lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer) - 1];
        } else { /* Slave node */
          /* checkparity and extrait PID */
          lnode_p->current_id= lin_process_parity(tmp_byte, CHECK_PARITY);
          lnode_p->pid = tmp_byte;
          if(lnode_p->current_id !=0xFF) {
            /*****************************************/
            /*** ID received correctly - parity OK ***/
            /*****************************************/
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
            /* Set Maximum response frame timeout */
            lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer) - 1];
          }
          else {
            /*****************************************/
            /*** ID Parity Error                   ***/
            /*****************************************/
            /* set lin status: parity_error */
            lnode_p->l_status.byte |= LIN_STA_PARITY_ERR;
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_ERR, 0x00);
            lin_goto_idle_state(channel);
          }
        }
        break;

      /******************************
      *** 4.4 SLAVE: Receiving data
      *******************************/
      case RECV_DATA:
        lnode_p->ptr++;
        *(lnode_p->ptr)=tmp_byte;
        /* Check bytes received fully */
        if(lnode_p->cnt_byte==(lnode_p->response_buffer[0])) {
          /* checksum checking */
          if(lin_checksum(lnode_p->response_buffer, lnode_p->pid)==tmp_byte) {
            /*******************************************/
            /***  RX Buffer Full - Checksum OK       ***/
            /*******************************************/
            /* set lin status: successful_transfer */
            lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
            /* disable RX interrupt */
            _pSCI->scicr2.byte &= ~(SCICR2_RE_MASK | SCICR2_RIE_MASK);
            lnode_p->state=PROC_CALLBACK;
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_RX_COMPLETED, lnode_p->current_id);
            /* enable RX interrupt */
            _pSCI->scicr2.byte |= (SCICR2_RE_MASK | SCICR2_RIE_MASK);
            if (SLEEP_MODE != lnode_p->state){
              lin_goto_idle_state(channel);
            }
          } else {
            /*******************************************/
            /***  RX Buffer Full - Checksum ERROR    ***/
            /*******************************************/
            /* set lin status: error_in_response, checksum_error */
            lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_CHECKSUM_ERR);
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_CHECKSUM_ERR, lnode_p->current_id);
            lin_goto_idle_state(channel);
          }
        }
        lnode_p->cnt_byte++;
        break;

      /******************************
      *** 4.5 SLAVE: Sending data
      *******************************/
        case SEND_DATA:
          /* Check for READBACK error */
          if (0==(sci_flag_sr1&SCISR1_TC_MASK)){
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
            lin_goto_idle_state(channel);
            break;
          }else{
            if (tmp_byte != lnode_p->response_buffer[lnode_p->cnt_byte]){
              CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
              lin_goto_idle_state(channel);
              break;
            }
          }

          if(lnode_p->cnt_byte <= (lnode_p->response_buffer[0])) {
            /* Send data bytes and checksum */
            lnode_p->cnt_byte++;
            _pSCI->scid.byte = lnode_p->response_buffer[lnode_p->cnt_byte];
          } else {
            /* TX transfer complete */
            lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
            /* Disable RX interrupt */
            _pSCI->scicr2.byte &= ~(SCICR2_RE_MASK|SCICR2_RIE_MASK);
            lnode_p->state = PROC_CALLBACK;
            /* trigger CALLBACK */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_TX_COMPLETED, lnode_p->current_id);
            /* Enable RX interrupt */
            _pSCI->scicr2.byte |= (SCICR2_RE_MASK|SCICR2_RIE_MASK);
            lin_goto_idle_state(channel);
          }
          break;
      /******************************
      *** 4.8 SLAVE: Low power mode
      *******************************/
      case SLEEP_MODE:
        if ((tmp_byte == 0xF0) || (tmp_byte == 0xE0) || (tmp_byte == 0xC0) || (tmp_byte == 0x80) || (tmp_byte == 0x00)) {
          /* Set idle timeout again */
          lin_goto_idle_state(channel);
        }
        break;
      default:
        break;
    }
  }
#endif /* End (SCI_VERSION = SCI_V2) */

/*--------------------------------------------------------------------*/

#if (SCI_VERSION == SCI_V4)
  l_u8 sci_flag_sr1;
  l_u8 sci_flag_sr2;
  l_u8 tmp_byte;

  register lin_node *lnode_p;     /* local pointer to the lin node descriptor */
  volatile tSCI* _pSCI;
  lnode_p = (lin_node *)&lin_node_descrs[channel];
  _pSCI = lnode_p->pSCI;

  sci_flag_sr2 = _pSCI->scisr2.byte ;

  if((sci_flag_sr2 & SCISR2_RXEDGIF_MASK) && (!(sci_flag_sr2 & SCISR2_LBKDIF_MASK)))
  {
    /* Clear flag */
    _pSCI->scisr2.bit.rxedgif = 1;
    /* check state of node is SLEEP_MODE */
    if (SLEEP_MODE == lnode_p->state){
      lin_goto_idle_state(channel);
    }

    /* Enable Break interrupt */
    _pSCI->scibdh.byte |= SCIBDH_LBKDIE_MASK ;
    /* Disable Active Edge interrupt */
    _pSCI->scibdh.byte &= ~SCIBDH_RXEDGIE_MASK;

    /* Receive data not inverted */
    _pSCI->scisr2.bit.rxinv = 0;
    return;
  }

 /* Check LBK flag */
  if (1 == _pSCI->scisr2.bit.lbkdif){
    /* Clear flag */
    _pSCI->scisr2.bit.lbkdif = 1;
    /* Enable Active Edge interrupt */
    _pSCI->scibdh.byte |= SCIBDH_RXEDGIE_MASK;
    /* Disable Break interrupt */
    _pSCI->scibdh.byte &= ~SCIBDH_LBKDIE_MASK ;
    /* check state of node is SLEEP_MODE */
    if (SLEEP_MODE == lnode_p->state){
      lin_goto_idle_state(channel);
      return;
    }
    /* reset lin status */
    lnode_p->l_status.byte = LIN_STA_BUS_ACTIVITY;
    /* Set max frame timeout */
    lnode_p->frame_timeout_cnt  = lin_max_frame_res_timeout_val[lnode_p->ifc][7];
    /******************************
    *** 1.1 MASTER NODE: Sending SYN field
    *******************************/
    if(lnode_p->func == 0/* Master */) {
      lnode_p->state=SEND_PID;
      /* Send syn field */
      _pSCI->scid.byte = 0x55;
    }
    /******************************
    *** 1.2 SLAVE NODE: Wait for SYN
    *******************************/
    else {
      lnode_p->state=RECV_SYN;
    }
    /* Disable LBK interrupt */
    _pSCI->scisr2.bit.lbkde = 0;
    return;
  } else {
    sci_flag_sr1 = lnode_p->pSCI->scisr1.byte;
    tmp_byte = lnode_p->pSCI->scid.byte;

    /******************************
    *** 4. BYTE RECIEVED
    *******************************/
    if(0 != (sci_flag_sr1&SCISR1_RDRF_MASK)) {

      switch(lnode_p->state) {
        /******************************
        *** 4.1 MASTER: Sending PID of frame
        *******************************/
        case SEND_PID:
          lnode_p->state=RECV_PID;
          /* Send PID byte */
          _pSCI->scid.byte = lnode_p->pid;
          break;
        /******************************
        *** 4.2 SLAVE: Receiving SYN byte
        *******************************/
        case RECV_SYN:
          if (0x55 == tmp_byte){
            lnode_p->state=RECV_PID;
          } else {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
#endif /* End of (LIN_PROTOCOL == PROTOCOL_J2602) */
          lin_goto_idle_state(channel);
        }
          break;
        /******************************
        *** 4.3 SLAVE: Receiving PID
        *******************************/
        case RECV_PID:
          if(!lnode_p->func) { /*Master*/
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
            /* Set Maximum response frame timeout */
            lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer) - 1];
          }
          else { /* Slave node */
            /* checkparity and extrait PID */
            lnode_p->current_id= lin_process_parity(tmp_byte, CHECK_PARITY);
            /* Keep the PID */
            lnode_p->pid = tmp_byte;
            if(lnode_p->current_id !=0xFF) {
              /*****************************************/
              /*** ID received correctly - parity OK ***/
              /*****************************************/
              /* trigger callback */
              CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
              /* Set Maximum response frame timeout */
              lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer) - 1];
            }
            else {
              /*****************************************/
              /*** ID Parity Error                   ***/
              /*****************************************/
              /* set lin status: parity_error */
              lnode_p->l_status.byte |= LIN_STA_PARITY_ERR;
              /* trigger callback */
              CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_ERR, 0xFF);
              lin_goto_idle_state(channel);
            }
          }
          break;

        /******************************
        *** 4.4 SLAVE: Receiving data
        *******************************/
        case RECV_DATA:
          lnode_p->ptr++;
          *(lnode_p->ptr)=tmp_byte;
          /* Check bytes received fully */
          if(lnode_p->cnt_byte==(lnode_p->response_buffer[0])) {
            /* checksum checking */
            if(lin_checksum(lnode_p->response_buffer, lnode_p->pid)==tmp_byte) {
              /*******************************************/
              /***  RX Buffer Full - Checksum OK       ***/
              /*******************************************/
              /* set lin status: successful_transfer */
              lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
              /* disable RX interrupt */
              _pSCI->scicr2.byte &= ~(SCICR2_RE_MASK | SCICR2_RIE_MASK);
              lnode_p->state=PROC_CALLBACK;
              /* trigger callback */
              CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_RX_COMPLETED, lnode_p->current_id);

              /* enable RX interrupt */
              _pSCI->scicr2.byte |= (SCICR2_RE_MASK | SCICR2_RIE_MASK);
              if (SLEEP_MODE != lnode_p->state){
                lin_goto_idle_state(channel);
              }
            } else {
              /*******************************************/
              /***  RX Buffer Full - Checksum ERROR    ***/
              /*******************************************/
              /* set lin status: error_in_response, checksum_error */
              lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_CHECKSUM_ERR);
              /* trigger callback */
              CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_CHECKSUM_ERR, lnode_p->current_id);

              lin_goto_idle_state(channel);
            }
          }
          lnode_p->cnt_byte++;
          break;

        /******************************
        *** 4.5 SLAVE: Sending data
        *******************************/
        case SEND_DATA:
          /* Check for READBACK error */
          if (0==(sci_flag_sr1&SCISR1_TC_MASK)){
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
            lin_goto_idle_state(channel);
            break;
          }else{
            if (tmp_byte != lnode_p->response_buffer[lnode_p->cnt_byte]){
              CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
              lin_goto_idle_state(channel);
              break;
            }
          }

          if(lnode_p->cnt_byte <= (lnode_p->response_buffer[0])) {
            /* Send data bytes and checksum */
            lnode_p->cnt_byte++;
            _pSCI->scid.byte = lnode_p->response_buffer[lnode_p->cnt_byte];
          } else {
            /* TX transfer complete */
            lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
            /* Disable RX interrupt */
            _pSCI->scicr2.byte &= ~(SCICR2_RE_MASK|SCICR2_RIE_MASK);
            lnode_p->state = PROC_CALLBACK;
            /* trigger CALLBACK */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_TX_COMPLETED, lnode_p->current_id);
            /* Enable RX interrupt */
            _pSCI->scicr2.byte |= (SCICR2_RE_MASK|SCICR2_RIE_MASK);
            lin_goto_idle_state(channel);
          }
          break;
         /******************************
        *** 4.8 SLAVE: Low power mode
        *******************************/
        case SLEEP_MODE:
          if ((tmp_byte == 0xF0) || (tmp_byte == 0xE0) || (tmp_byte == 0xC0) || (tmp_byte == 0x80) || (tmp_byte == 0x00)) {
             /* Set idle timeout again */
            lin_goto_idle_state(channel);
             /* disable LIN break detect interrupt */
            _pSCI->scisr2.bit.lbkde = 1;
          }
          break;
        default:
          break;
      }
    }
  }
#endif /* End (SCI_VERSION = SCI_V4) */
} /* End function lin_lld_sci_rx_isr(sci_channel_name  channel) */

#if (SCI_VERSION == SCI_V5)
void lin_lld_sci_isr
  (
    /* [IN] sci channel name */
    sci_channel_name  channel
  )
{
  l_u8 sci_flag_sr1;
  l_u8 tmp_byte;

  register lin_node *lnode_p;     /* local pointer to the lin node descriptor */
  volatile tSCI* _pSCI;           /* pointer to the SCI peripheral */

  lnode_p = (lin_node *)&lin_node_descrs[channel];
  _pSCI   = lnode_p->pSCI;

  sci_flag_sr1 = _pSCI->scisr1.byte;

  /******************************
  *** 1. BREAK DETECTED
  *******************************/
  if(0 != (_pSCI->sciasr1.byte&SCIASR1_BKDIF_MASK)) {
    /* clear the error flag */
    _pSCI->sciasr1.byte |=SCIASR1_BKDIF_MASK;
    /* check state of node is SLEEP_MODE */
    if (SLEEP_MODE == lnode_p->state){
      lin_goto_idle_state(channel);
      return;
    }
    /* reset lin status */
    lnode_p->l_status.byte = LIN_STA_BUS_ACTIVITY;
    /* Set max frame timeout */
    lnode_p->frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][7];
    /******************************
    *** 1.1 MASTER NODE: Sending SYN field
    *******************************/
    if(lnode_p->func == 0/* Master */) {
      lnode_p->state=SEND_PID;
      /* Send syn field */
      _pSCI->scidrl.byte = 0x55;
    }
    /******************************
    *** 1.2 SLAVE NODE: Wait for SYN
    *******************************/
    else {
      lnode_p->state=RECV_SYN;
    }
    return;
  }
  /******************************
  *** 2. BIT ERROR DETECTED
  *******************************/
  else if(0 != (_pSCI->sciasr1.byte&SCIASR1_BERRIF_MASK)) {
    _pSCI->sciasr1.byte |= SCIASR1_BERRIF_MASK;
    /* set lin status: error_in_response, readback_error */
    lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_READBACK_ERR);
    if((lnode_p->state == SEND_DATA)||(lnode_p->state == SEND_DATA_COMPLETED))
      /* trigger callback */
      CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);

    lin_goto_idle_state(channel);
    return;
  }

  /******************************
  *** 3. FRAME ERROR DETECTED
  *******************************/
  if(0 != (sci_flag_sr1&SCISR1_FE_MASK)) {
    /* Clear FE flags */
    _pSCI->scisr1.byte |= SCISR1_FE_MASK;
    tmp_byte = _pSCI->scidrl.byte;
    /* set lin status: error_in_response, framing_error */
    lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_FRAME_ERR);
    /* trigger callback */
    if((lnode_p->state == RECV_DATA)||(lnode_p->state == SEND_DATA) || (lnode_p->state == SEND_DATA_COMPLETED)){
      CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_FRAME_ERR, lnode_p->current_id);
    }
    lin_goto_idle_state(channel);
  }
  /******************************
  *** 4. BYTE RECIEVED
  *******************************/
  else if(0 != (sci_flag_sr1&SCISR1_RDRF_MASK)) {
    tmp_byte = _pSCI->scidrl.byte;
    switch(lnode_p->state) {
      /******************************
      *** 4.1 MASTER: Sending PID of frame
      *******************************/
      case SEND_PID:
        lnode_p->state=RECV_PID;
        /* Send PID byte */
        _pSCI->scidrl.byte = lnode_p->pid;
        break;
      /******************************
      *** 4.2 SLAVE: Receiving SYN byte
      *******************************/
      case RECV_SYN:
        if (0x55 == tmp_byte){
          lnode_p->state=RECV_PID;
        } else {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
#endif /* End of (LIN_PROTOCOL == PROTOCOL_J2602) */
          lin_goto_idle_state(channel);
        }
        break;
      /******************************
      *** 4.3 SLAVE: Receiving PID
      *******************************/
      case RECV_PID:
        if(!lnode_p->func) { /*Master*/
          /* trigger callback */
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
          /* Set Maximum response frame timeout */
          lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer)-1];
        }
        else { /* Slave node */
          /* checkparity and extrait PID */
          lnode_p->pid=tmp_byte;
          lnode_p->current_id = lin_process_parity(tmp_byte,CHECK_PARITY);
          if(lnode_p->current_id !=0xFF) {
            /*****************************************/
            /*** ID received correctly - parity OK ***/
            /*****************************************/
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
            /* Set Maximum response frame timeout */
            lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer)-1];
          }
          else {
            /*****************************************/
            /*** ID Parity Error                   ***/
            /*****************************************/
            /* set lin status: parity_error */
            lnode_p->l_status.byte |= LIN_STA_PARITY_ERR;
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_ERR, 0xFF);
            lin_goto_idle_state(channel);
          }
        }
        break;

      /******************************
      *** 4.4 SLAVE: Receiving data
      *******************************/
      case RECV_DATA:
        lnode_p->ptr++;
        *(lnode_p->ptr)=tmp_byte;
        /* Check bytes received fully */
        if(lnode_p->cnt_byte==(lnode_p->response_buffer[0])) {
          /* checksum checking */
          if(lin_checksum(lnode_p->response_buffer, lnode_p->pid)==tmp_byte) {
            /*******************************************/
            /***  RX Buffer Full - Checksum OK       ***/
            /*******************************************/
            /* set lin status: successful_transfer */
            lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
            /* disable RX interrupt */
            _pSCI->scicr2.byte &= ~(SCICR2_RE_MASK | SCICR2_RIE_MASK);
            lnode_p->state=PROC_CALLBACK;
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_RX_COMPLETED, lnode_p->current_id);

            /* enable RX interrupt */
            _pSCI->scicr2.byte |= (SCICR2_RE_MASK | SCICR2_RIE_MASK);
            if (SLEEP_MODE != lnode_p->state){
              lin_goto_idle_state(channel);
            }
          } else {
            /*******************************************/
            /***  RX Buffer Full - Checksum ERROR    ***/
            /*******************************************/
            /* set lin status: error_in_response, checksum_error */
            lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_CHECKSUM_ERR);
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_CHECKSUM_ERR, lnode_p->current_id);

            lin_goto_idle_state(channel);
          }
        }
        lnode_p->cnt_byte++;
        break;

      /******************************
      *** 4.5 SLAVE: Sending data
      *******************************/
      case SEND_DATA:
        if (0 == (sci_flag_sr1&SCISR1_TC_MASK )) {
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
          lnode_p->state = PROC_CALLBACK;
          lin_goto_idle_state(channel);
          break;
        }
        if(lnode_p->cnt_byte==(lnode_p->response_buffer[0])) {
          /* Send checksum byte */
          _pSCI->scidrl.byte = lnode_p->response_buffer[lnode_p->cnt_byte+1];
          lnode_p->state= SEND_DATA_COMPLETED;
        } else {
          /* Send byte next */
          lnode_p->cnt_byte++;
          _pSCI->scidrl.byte = lnode_p->response_buffer[lnode_p->cnt_byte];
        }
        break;
      /******************************
      *** 4.7 SLAVE: Sending data compeleted
      *******************************/
      case SEND_DATA_COMPLETED:
        /*******************************************/
        /***  TX Buffer Empty - Checksum Sent    ***/
        /*******************************************/
        if (0 == (sci_flag_sr1&SCISR1_TC_MASK )) {
          CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
          lnode_p->state = PROC_CALLBACK;
          lin_goto_idle_state(channel);
          break;
        }
        /* set lin status: successful_transfer */
        lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
        /* disable RX interrupt */
        _pSCI->scicr2.byte &= ~(SCICR2_RE_MASK | SCICR2_RIE_MASK);
        lnode_p->state=PROC_CALLBACK;
        /* trigger callback */
        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_TX_COMPLETED, lnode_p->current_id);

        _pSCI->scicr2.byte |= (SCICR2_RE_MASK | SCICR2_RIE_MASK);
        /* enable RX interrupt */
        lin_goto_idle_state(channel);
        break;
      /******************************
      *** 4.8 SLAVE: Low power mode
      *******************************/
      case SLEEP_MODE:
        if ((tmp_byte == 0xF0) || (tmp_byte == 0xE0) || (tmp_byte == 0xC0) || (tmp_byte == 0x80) || (tmp_byte == 0x00)) {
          /* Set idle timeout again */
          lin_goto_idle_state(channel);
        }
        break;
      default:
        break;
    }
  }
} /* End function lin_lld_sci_isr(sci_channel_name  channel) */
#endif /* End (SCI_VERSION = SCI_V5) */


#endif /* End (LIN_MODE == _MASTER_MODE_) */

/**
* @}
*/
