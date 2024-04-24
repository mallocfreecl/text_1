/****************************************************************************** 
* Freescale Semiconductor Inc. 
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
*****************************************************************************/ 

/** @addtogroup common_core_api_group
* @{ */

/**************************************************************************//** 
* 
* @file      lin_common_api.c 
* 
* @author    FPT Software
*  
* @brief     Common LIN API functions
* 
* 
******************************************************************************/
/******************************************************************************
 *
 * History:
 *
 * 20090408		v1.0	First version
 *
 *****************************************************************************/
 
/** @} */

#include "lin_common_api.h"
#include "lin_common_proto.h"
#include "lin_sw.h"

#if (XGATE_SUPPORT == 1)
#include "lin_lld_xgate.h"
#endif /* End (XGATE_SUPPORT == 1) */

/*-------------------------------FUNCTION COMMON ---------------------------*/
/** @addtogroup driver_cluster_group 
* @{ */
l_bool l_sys_init ()
{
#if XGATE_SUPPORT == 1
  lin_lld_setup_xgate();
#endif /* End (XGATE_SUPPORT == 1) */
  lin_lld_timer_init();
  return 0;
} /* end of l_sys_init() */
/** @} */

/** @addtogroup signal_interract_group
* @{
*/
l_bool l_bool_rd
  (
    /*[IN] signal name */
    l_signal_handle sss
	)
{
  return (l_bool)lin_signal_tbl[sss].sig_data[0];
} /* end of l_bool_rd() */

l_u8 l_u8_rd
  (
    /*[IN] signal name */
    l_signal_handle sss
  )
{
  return lin_signal_tbl[sss].sig_data[0];
} /* end of l_u8_rd() */

l_u16 l_u16_rd 
  (
    /*[IN] signal name */
    l_signal_handle sss
  )
{
  return (((l_u16)lin_signal_tbl[sss].sig_data[0]) << 8 ) | ((l_u16) lin_signal_tbl[sss].sig_data[1]);
} /* end of l_u16_rd() */

void l_bytes_rd
  (
    /* [IN] signal name */
    l_signal_handle sss,
    /* [IN] first byte to read from */
    l_u8 start,
    /* [IN] number of bytes to read */
    l_u8 count,
    /* [OUT] output data */
    l_u8* const data
  )
{
  l_u8 i,j;
  
  for (i = count+start, j = start; i > start; i--, j++ ) {
    data[i-1] = lin_signal_tbl[sss].sig_data[j];
  }
} /* end of l_bytes_rd() */

void l_u8_index_wr 
  ( 
    l_u8 index
  ) 
{
  frm_index_b = index;
} 

void l_bool_wr
  (
    /* [IN] signal name */
    l_signal_handle sss,    
    /* [IN] input value */    
    l_bool v
  ) 
{
  l_bool_wr_b(sss,v,frm_index_b);   
} /* end of l_bool_wr() */

void l_bool_wr_b
  (
    /* [IN] signal name */
    l_signal_handle sss,    
    /* [IN] input value */    
    l_bool v,
    l_u8 frm_index
  ) 
{
  /* update frame and signal flags */
	lin_update_frame_flag(sss,frm_index);
  lin_signal_tbl[sss].sig_data[0] = (l_u8)v;    
} /* end of l_bool_wr() */

void l_u8_wr 
  ( 
    /* [IN] signal name */     
    l_signal_handle sss,
    /* [IN] input value */
    l_u8 v
  ) 
{
  l_u8_wr_b(sss,v,frm_index_b);
} /* end of l_u8_wr() */

void l_u8_wr_b 
  ( 
    /* [IN] signal name */     
    l_signal_handle sss,
    /* [IN] input value */
    l_u8 v,
    l_u8 frm_index
  ) 
{
	/* update frame and signal flags */
	lin_update_frame_flag(sss,frm_index);
	lin_signal_tbl[sss].sig_data[0] = v;
} /* end of l_u8_wr() */

void l_u16_wr 
  (
    /* [IN] signal name */
    l_signal_handle sss,
    /* [IN] input value */
    l_u16 v 
  ) 
{
  l_u16_wr_b(sss,v,frm_index_b);
} /* end of l_u16_wr() */

void l_u16_wr_b
  (
    /* [IN] signal name */
    l_signal_handle sss,
    /* [IN] input value */
    l_u16 v,
    l_u8 frm_index
  ) 
{
	/* update frame and signal flags */
	lin_update_frame_flag(sss,frm_index);
	lin_signal_tbl[sss].sig_data[0] = (v >> 8);
	lin_signal_tbl[sss].sig_data[1] = (v & 0x00FF);
}

void l_bytes_wr
  (
    /* [IN] signal name */
    l_signal_handle sss,
    /* [IN] first byte to write from */
    l_u8 start,
    /* [IN] number of bytes to write */
    l_u8 count,
    /* [IN] data will be read from */
    l_u8* const data
  )
{
  l_bytes_wr_b(sss,start,count,data,frm_index_b);
	
} /* end of l_bytes_wr() */

void l_bytes_wr_b
  (
    /* [IN] signal name */
    l_signal_handle sss,
    /* [IN] first byte to write from */
    l_u8 start,
    /* [IN] number of bytes to write */
    l_u8 count,
    /* [IN] data will be read from */
    l_u8* const data,
		l_u8 frm_index
  )
{
  l_u8 i,j;
	/* update frame and signal flags */
	lin_update_frame_flag(sss,frm_index);
	for (i = count+start, j = start; i > start; i--, j++ ) {
		lin_signal_tbl[sss].sig_data[i-1] = data[j];
	}
} /* end of l_bytes_wr() */
/** @} */

/** @addtogroup notification_group 
* @{ */
l_bool l_flg_tst 
  (
    /* [IN] flag name */
    l_flag_handle fff
  )
{    
  
  /* Frame flag */      
  if(0x00FF ==(fff&0x00FF))
    return lin_flag_frame_handle_tbl[(fff & 0xFF00)>>8];
  /* Signal tx flag  */
  else if(0xFF00 ==(fff&0xFF00))
    return lin_flag_signal_handle_tbl[(l_u8)fff];
  return 0;
} /* end of l_flg_tst() */

void l_flg_clr 
  (
    /* [IN] flag name */
    l_flag_handle fff
  )
{
  /* Frame flag */      
  if(0x00FF ==(fff&0x00FF))
    lin_flag_frame_handle_tbl[(fff & 0xFF00)>>8] = 0;
  /* Signal flag  */
  else if(0xFF00 ==(fff&0xFF00))
    lin_flag_signal_handle_tbl[(l_u8)fff] = 0;
} /* end of l_flg_clr() */
/** @} */
#if LIN_MODE == _MASTER_MODE_
/** @addtogroup schedule_management_group 
* @{ */
void l_sch_set
    (
        /* [IN] interface name */
        l_ifc_handle iii,
        /* [IN] schedule table for interface */
        l_schedule_handle schedule_iii,
        /* [IN] entry to be set */
        l_u8 entry
	)
{
  lin_configuration const *conf;
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  /* save the new schedule which is activated after the      */
  /* current schedule reaches its next schedule entry point  */
  *(conf->previous_schedule_id) = *(conf->active_schedule_id);
  /* Insert new schedule at that point of the current schdule*/
  *(conf->active_schedule_id) = (l_u8)schedule_iii;
  /*Assign the start entry of new schedule table by argument entry */
  conf->schedule_start_entry[schedule_iii] = entry;
} /* end of l_sch_set() */

l_u8 l_sch_tick 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  return lin_tick_callback_handler(iii);
} /* end of l_sch_tick() */
/** @} */
#endif /* End LIN_MODE == _MASTER_MODE_*/

/** @addtogroup interface_management_group 
* @{ */
l_bool l_ifc_init 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  return lin_lld_init();
#else
  return lin_lld_init(iii);
#endif /* End LIN_MODE == _SLAVE_MODE_*/
} /* end of l_ifc_init() */

void l_ifc_goto_sleep 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  /* If node is master node */
#if LIN_MODE == _MASTER_MODE_
  /* Get current configuration */
  lin_configuration const *conf; 
  /* Get current configuration */
  conf = &lin_ifc_configuration[iii];
  /* Set active schedule as GOTO_SLEEP_SCHEDULE */
  l_sch_set(iii, (l_schedule_handle)(conf->schedule_start + 1), 0); 
  /* Update goto_sleep flag */
  (*conf->goto_sleep_flg) = 1;
  
#endif /*End LIN_MODE == _MASTER_MODE_ */    
} /* end of l_ifc_goto_sleep() */

void l_ifc_wake_up
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_tx_wake_up();
#else
  /* Clear goto sleep flag */
  if(1 == (*lin_ifc_configuration[iii].goto_sleep_flg)){
    (*lin_ifc_configuration[iii].goto_sleep_flg) = 0;
  }
  
  /* Send wakeup signal */
  lin_lld_tx_wake_up(iii);

#endif /* End (LIN_MODE == _SLAVE_MODE_) */ 
} /* end of l_ifc_wake_up() */

void l_ifc_rx
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{

} /* end of l_ifc_rx() */
  
void l_ifc_tx 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{

} /* end of l_ifc_tx() */

void l_ifc_aux 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
 
} /* end of l_ifc_aux() */
l_u16 l_ifc_read_status
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
  l_u16 tmp_word_status;
#if LIN_MODE == _SLAVE_MODE_
  tmp_word_status = lin_word_status.word;
  /* Clear word status */
  lin_word_status.word = 0;
  /* Clear save configuration flag value */
  lin_save_configuration_flg = 0;
#else
  lin_configuration const *conf;
  conf = &lin_ifc_configuration[iii];
  tmp_word_status = (l_u16)conf->word_status;
  /* Clear word status */
  conf->word_status->word = 0;
  /* Clear save configuration flag value */
  lin_save_configuration_flg[iii] = 0;
#endif /* End (LIN_MODE == _SLAVE_MODE_) */

  return (tmp_word_status);
} /* end of l_ifc_read_status() */
/** @} */

/** @addtogroup call_out_group 
* @{ */
l_u16 l_sys_irq_disable 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  return lin_lld_int_disable();
#else
  return lin_lld_int_disable(iii);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
} /* end of l_sys_irq_disable() */

void l_sys_irq_restore 
  (
    /* [IN] interface name */
    l_ifc_handle iii
  )
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_int_enable();
#else
  lin_lld_int_enable(iii);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
} /* end of l_sys_irq_restore() */
 /** @} */

/********************** Seperate Function *********************************/
/** @addtogroup common_core_api_group
* @{ */
/* ---------------------------- For 1 interface -----------------------------------*/
#if LIN_MODE == _SLAVE_MODE_

void lin_update_frame_flag
  (
    /* [IN] signal name */
    l_signal_handle sss,
    l_u8 frm_index
  )
{
  /*Update signal flag*/
  lin_signal_flag_tbl[sss] = 1;
  /*Update frame flag*/
	if(lin_frame_tbl[frm_index].frm_type == LIN_FRM_UNCD)
	{
	  lin_frame_flag_tbl[frm_index] = 1;			
	}
}
#endif /* end of LIN_MODE == _SLAVE_MODE_*/

/* ---------------------------- For multi interface -----------------------------------*/

#if LIN_MODE == _MASTER_MODE_


void lin_update_frame_flag
  (
    /* [IN] LIN configuration */
    lin_configuration *conf_ptr,
    /* [IN] signal name */
    l_signal_handle sss
  ) 
{
  l_u8 i;
  l_s8 j, k;
  lin_frm2sig *ptr;

  /*Update signal flag*/
  lin_signal_flag_tbl[sss] = 1;
  /*Update frame flag*/
  for(i = conf_ptr->frame_start, k=conf_ptr->num_of_frames; 0 < k; k--, i++) {
    /* Only check with unconditional frame */
    if(lin_frame_tbl[i].frm_type == LIN_FRM_UNCD){
      /* get frame data */
      ptr = (lin_frm2sig *)(lin_frame_tbl[i].frame_data);
      for(j = lin_frame_tbl[i].num_of_sig-1; ((l_s8)0)<= j; j--){
        if((ptr[j].sig_id) == sss){
          /* update frame flag */
          lin_frame_flag_tbl[i] = 1;
        }
      }
    }
  }
} /* end of lin_update_frame_flag() */


l_ifc_handle lin_find_associate_ifc
  (
    /* [IN] interface name */
    l_signal_handle sss
  ) 
{
  l_u8 i, signal_start;
  l_ifc_handle found;
  
  found = (l_ifc_handle)0;
  /* find the interface */
  for (i = 0; i < LIN_NUM_OF_IFCS; i++) {
    signal_start = lin_ifc_configuration[i].signal_start;
    if ( (signal_start < sss) && (signal_start > lin_ifc_configuration[found].signal_start)){
      found = (l_ifc_handle)i;
    }
  }
  return found;
} /* end of lin_find_associate_ifc() */
#endif /* end of LIN_MODE == _MASTER_MODE_*/
/** @} */
