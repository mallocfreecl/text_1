/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2020-2021 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
***************************************************************************//*! 
* 
* @file      lin_cfg.c 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Tue Oct 19 13:51:43 CST 2021 
*  
* @brief     Common LIN configuration, data structure
* 
******************************************************************************/
#include "lin_cfg.h"
#include "lin_sw.h"
/* Mapping interface with hardware */
const lin_hardware_name lin_virtual_ifc = SCI0; 
l_u8 lin_lld_response_buffer[10];
 
l_u8 lin_successful_transfer; 
l_u8 lin_error_in_response; 
l_u8 lin_goto_sleep_flg; 
/* Save configuration flag */
l_u8 lin_save_configuration_flg = 0;
lin_word_status_str lin_word_status; 
l_u8 lin_current_pid; 

/******************************* Signal Data **********************************/

l_u8 LI0_BEWP_RefSpd_data[1] = {0x00};

l_u8 LI0_BEWP_TargetSpd_data[1] = {0x00};

l_u8 LI0_BEWP_ActSpd_data[1] = {0x00};

l_u8 LI0_BEWP_Voltage_data[1] = {0x00};

l_u8 LI0_BEWP_Temperature_data[1] = {0x00};

l_u8 LI0_BEWP_Current_data[1] = {0x00};

l_u8 LI0_BEWP_OverTempSts_data[1] = {0x00};

l_u8 LI0_BEWP_OverCurrentSts_data[1] = {0x00};

l_u8 LI0_BEWP_DryRunningSts_data[1] = {0x00};

l_u8 LI0_BEWP_VoltageSts_data[1] = {0x00};

l_u8 LI0_BEWP_BlockSts_data[1] = {0x00};

l_u8 LI0_BEWP_RESP_ERROR_data[1] = {0x00};

/******************************* Signal Table **********************************/
const lin_signal_struct  lin_signal_tbl[LIN_NUM_OF_SIGS] ={
 /*sig_type        sig_size_bit      sig_data*/

   {LIN_SIG_SCALAR, 8, &LI0_BEWP_RefSpd_data[0]}
  

   ,{LIN_SIG_SCALAR, 8, &LI0_BEWP_TargetSpd_data[0]}
  
   ,{LIN_SIG_SCALAR, 8, &LI0_BEWP_ActSpd_data[0]}
  
   ,{LIN_SIG_SCALAR, 8, &LI0_BEWP_Voltage_data[0]}
  
   ,{LIN_SIG_SCALAR, 8, &LI0_BEWP_Temperature_data[0]}
  
   ,{LIN_SIG_SCALAR, 8, &LI0_BEWP_Current_data[0]}
  
   ,{LIN_SIG_SCALAR, 2, &LI0_BEWP_OverTempSts_data[0]}
  
   ,{LIN_SIG_SCALAR, 2, &LI0_BEWP_OverCurrentSts_data[0]}
  
   ,{LIN_SIG_SCALAR, 2, &LI0_BEWP_DryRunningSts_data[0]}
  
   ,{LIN_SIG_SCALAR, 2, &LI0_BEWP_VoltageSts_data[0]}
  
   ,{LIN_SIG_SCALAR, 2, &LI0_BEWP_BlockSts_data[0]}
  
   ,{LIN_SIG_SCALAR, 1, &LI0_BEWP_RESP_ERROR_data[0]}
  
};
/*************************** Flag set when signal is updated ******************/
l_bool lin_signal_flag_tbl[LIN_NUM_OF_SIGS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/* Flag is to notify to application whether signal is transmited or received successfully */
l_bool lin_flag_signal_handle_tbl[LIN_NUM_OF_SIGS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/* Diagnostic signal */
l_u8 lin_diag_signal_tbl[16];
/***************************** Frame Definition ******************************/
/*unconditional frame */

const lin_frm2sig LI0_VCU_BEWP_signal[1] = {

   {LI0_BEWP_RefSpd, 0}
};

const lin_frm2sig LI0_BEWP_RESP_signal[11] = {

   {LI0_BEWP_TargetSpd, 0}

   ,{LI0_BEWP_ActSpd, 8} 

   ,{LI0_BEWP_Voltage, 16} 

   ,{LI0_BEWP_Temperature, 24} 

   ,{LI0_BEWP_Current, 32} 

   ,{LI0_BEWP_OverTempSts, 46} 

   ,{LI0_BEWP_OverCurrentSts, 48} 

   ,{LI0_BEWP_DryRunningSts, 50} 

   ,{LI0_BEWP_VoltageSts, 52} 

   ,{LI0_BEWP_BlockSts, 54} 

   ,{LI0_BEWP_RESP_ERROR, 63} 

};



/*****************************event trigger frame*****************************/

/**********************************  Frame table **********************************/
const lin_frame_struct lin_frame_tbl[LIN_NUM_OF_FRMS] ={

   { LIN_FRM_UNCD, 2, LIN_RES_SUB, 1  , (l_u8*)&LI0_VCU_BEWP_signal[0]  }

   ,{ LIN_FRM_UNCD, 8, LIN_RES_PUB, 11 , (l_u8*)&LI0_BEWP_RESP_signal[0] }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_SUB, 8 , (l_u8*)0 }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_PUB, 8 , (l_u8*)0 }
  
};
/*********************************** Frame flag Initialization **********************/
l_bool lin_frame_flag_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0};
l_bool lin_flag_frame_handle_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0};


/**************************** Lin configuration Initialization ***********************/
/* max_response_frame_timeout = round((1.4x(10+Nx10)xTbit)/Tbase_period) + 3 */

const l_u16 lin_max_frame_res_timeout_val[8]={

6, 7, 9, 10, 12, 13, 15, 16

};


  

l_u8 lin_configuration_RAM[LIN_SIZE_OF_CFG]= {0x00, 0x11, 0x1B, 0x3C, 0x3D ,0xFF};
const l_u16  lin_configuration_ROM[LIN_SIZE_OF_CFG]= {0x00, 0x11, 0x1B, 0x3C, 0x3D ,0xFFFF};


/***************************************** Node Attribute*****************************************/
   
l_u8 lin_configured_NAD = 0x35;     /*<configured_NAD>*/ 
const l_u8 lin_initial_NAD    =0x35;     /*<initial_NAD>*/ 
const lin_product_id product_id = {0x0124, 0x0501, 0x0001 };  /* {<supplier_id>,<function_id>,<variant>} */ 
const l_signal_handle response_error =  LI0_BEWP_RESP_ERROR;               




/************************** TL Layer and Diagnostic: SINGLE interface **************************/
lin_tl_pdu_data tx_single_pdu_data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
lin_tl_pdu_data rx_single_pdu_data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};





 

lin_tl_pdu_data *tl_current_tx_pdu_ptr;
lin_tl_pdu_data *tl_current_rx_pdu_ptr;
l_u8 tl_slaveresp_cnt = 0;
 
/* This function is an example of response; real implementation is application-dependent */
/* You can use one of the following define to set PCI of response frame for */
/* this service to the correct value */
l_u8 ld_read_by_id_callout(l_u8 id, l_u8 *data){
    l_u8 retval = LD_ID_NO_RESPONSE;
    /* Following code is an example - Real implementation is application-dependent */
    /*
     * the handling does essentially depend on the id of the
     * requested item
     */
    /* This example implement with ID = 32 - LIN_READ_USR_DEF_MIN */
    if (id == LIN_READ_USR_DEF_MIN)
    {
      /*
       * id received is user defined 32
       */
      /* A positive response is ready to be sent to the user defined request */
     
      data[0] = (l_u8) (id + 1);    /* Data user define */
      data[1] = (l_u8) (id + 2);    /* Data user define */
      data[2] = (l_u8) (id + 3);    /* Data user define */
      data[3] = (l_u8) (id + 4);    /* Data user define */
      data[4] = (l_u8) (id + 5);    /* Data user define */
      retval = LD_POSITIVE_RESPONSE;
    }
    else 
    {
      /*
       * id received is user defined 63 - no response
       */
    }
    return retval;    
}
