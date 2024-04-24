/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2020-2021 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
****************************************************************************//*!  
* 
* @file      lin_cfg.h 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Tue Oct 19 13:51:43 CST 2021
*  
* @brief     Hardware configuration file
* 
******************************************************************************/
#ifndef    _LIN_CFG_H_  
#define    _LIN_CFG_H_  
#include "lin_hw_cfg.h" 
/* Define operating mode */
#define _MASTER_MODE_     0 
#define _SLAVE_MODE_      1 
#define LIN_MODE   _SLAVE_MODE_
/* Define protocol version */
#define PROTOCOL_21       0  
#define PROTOCOL_J2602    1  
#define PROTOCOL_20       2 
#define LIN_PROTOCOL    PROTOCOL_21

#define SCI_ADDR        SCI0_ADDR    /* For slave */ 
#define LIN_BAUD_RATE    19200    	 /*For slave*/ 
/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    _DIAG_CLASS_I_
/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_SINGLE_FRAME_

/**********************************************************************/
/***************               Flags             **********************/
/**********************************************************************/
typedef unsigned int l_flag_handle;
/* Signal flag */

#define LI0_BEWP_RefSpd_flag 0xFF00

#define LI0_BEWP_TargetSpd_flag 0xFF01

#define LI0_BEWP_ActSpd_flag 0xFF02

#define LI0_BEWP_Voltage_flag 0xFF03

#define LI0_BEWP_Temperature_flag 0xFF04

#define LI0_BEWP_Current_flag 0xFF05

#define LI0_BEWP_OverTempSts_flag 0xFF06

#define LI0_BEWP_OverCurrentSts_flag 0xFF07

#define LI0_BEWP_DryRunningSts_flag 0xFF08

#define LI0_BEWP_VoltageSts_flag 0xFF09

#define LI0_BEWP_BlockSts_flag 0xFF0A

#define LI0_BEWP_RESP_ERROR_flag 0xFF0B
 

/**********Frame flag definition **********/
/* Frame flag */

#define  LI0_VCU_BEWP_flag  0x00FF

#define  LI0_BEWP_RESP_flag  0x01FF

#define  LI0_MasterReq_flag  0x02FF

#define  LI0_SlaveResp_flag  0x03FF

/* Signal and Frame association */
/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
typedef enum { 
   LI0
}l_ifc_handle; 

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  12
/* List of signals */   
typedef enum {

   /* Interface_name = LI0 */

   LI0_BEWP_RefSpd

   , LI0_BEWP_TargetSpd
  
   , LI0_BEWP_ActSpd
  
   , LI0_BEWP_Voltage
  
   , LI0_BEWP_Temperature
  
   , LI0_BEWP_Current
  
   , LI0_BEWP_OverTempSts
  
   , LI0_BEWP_OverCurrentSts
  
   , LI0_BEWP_DryRunningSts
  
   , LI0_BEWP_VoltageSts
  
   , LI0_BEWP_BlockSts
  
   , LI0_BEWP_RESP_ERROR
  
  
} l_signal_handle; 
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  4 
/* List of frames */
typedef enum {
/* All frames for master node */

   /* Interface_name = LI0 */

   LI0_VCU_BEWP

   , LI0_BEWP_RESP
  
   , LI0_MasterReq
  
   , LI0_SlaveResp
  
  
} l_frame_handle; 
 
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  6 
#define LIN_CFG_FRAME_NUM  2 /**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/ 

/* SIGNAL INTERACTION */


/* Scalar signal read */
#define l_bool_rd_LI0_BEWP_RefSpd() l_bool_rd(LI0_BEWP_RefSpd)

#define l_bool_rd_LI0_BEWP_TargetSpd() l_bool_rd(LI0_BEWP_TargetSpd)

#define l_bool_rd_LI0_BEWP_ActSpd() l_bool_rd(LI0_BEWP_ActSpd)

#define l_bool_rd_LI0_BEWP_Voltage() l_bool_rd(LI0_BEWP_Voltage)

#define l_bool_rd_LI0_BEWP_Temperature() l_bool_rd(LI0_BEWP_Temperature)

#define l_bool_rd_LI0_BEWP_Current() l_bool_rd(LI0_BEWP_Current)

#define l_bool_rd_LI0_BEWP_OverTempSts() l_bool_rd(LI0_BEWP_OverTempSts)

#define l_bool_rd_LI0_BEWP_OverCurrentSts() l_bool_rd(LI0_BEWP_OverCurrentSts)

#define l_bool_rd_LI0_BEWP_DryRunningSts() l_bool_rd(LI0_BEWP_DryRunningSts)

#define l_bool_rd_LI0_BEWP_VoltageSts() l_bool_rd(LI0_BEWP_VoltageSts)

#define l_bool_rd_LI0_BEWP_BlockSts() l_bool_rd(LI0_BEWP_BlockSts)

#define l_bool_rd_LI0_BEWP_RESP_ERROR() l_bool_rd(LI0_BEWP_RESP_ERROR)



#define l_u8_rd_LI0_BEWP_RefSpd() l_u8_rd(LI0_BEWP_RefSpd)

#define l_u8_rd_LI0_BEWP_TargetSpd() l_u8_rd(LI0_BEWP_TargetSpd)

#define l_u8_rd_LI0_BEWP_ActSpd() l_u8_rd(LI0_BEWP_ActSpd)

#define l_u8_rd_LI0_BEWP_Voltage() l_u8_rd(LI0_BEWP_Voltage)

#define l_u8_rd_LI0_BEWP_Temperature() l_u8_rd(LI0_BEWP_Temperature)

#define l_u8_rd_LI0_BEWP_Current() l_u8_rd(LI0_BEWP_Current)

#define l_u8_rd_LI0_BEWP_OverTempSts() l_u8_rd(LI0_BEWP_OverTempSts)

#define l_u8_rd_LI0_BEWP_OverCurrentSts() l_u8_rd(LI0_BEWP_OverCurrentSts)

#define l_u8_rd_LI0_BEWP_DryRunningSts() l_u8_rd(LI0_BEWP_DryRunningSts)

#define l_u8_rd_LI0_BEWP_VoltageSts() l_u8_rd(LI0_BEWP_VoltageSts)

#define l_u8_rd_LI0_BEWP_BlockSts() l_u8_rd(LI0_BEWP_BlockSts)

#define l_u8_rd_LI0_BEWP_RESP_ERROR() l_u8_rd(LI0_BEWP_RESP_ERROR)




#define l_u16_rd_LI0_BEWP_RefSpd() l_u16_rd(LI0_BEWP_RefSpd)

#define l_u16_rd_LI0_BEWP_TargetSpd() l_u16_rd(LI0_BEWP_TargetSpd)

#define l_u16_rd_LI0_BEWP_ActSpd() l_u16_rd(LI0_BEWP_ActSpd)

#define l_u16_rd_LI0_BEWP_Voltage() l_u16_rd(LI0_BEWP_Voltage)

#define l_u16_rd_LI0_BEWP_Temperature() l_u16_rd(LI0_BEWP_Temperature)

#define l_u16_rd_LI0_BEWP_Current() l_u16_rd(LI0_BEWP_Current)

#define l_u16_rd_LI0_BEWP_OverTempSts() l_u16_rd(LI0_BEWP_OverTempSts)

#define l_u16_rd_LI0_BEWP_OverCurrentSts() l_u16_rd(LI0_BEWP_OverCurrentSts)

#define l_u16_rd_LI0_BEWP_DryRunningSts() l_u16_rd(LI0_BEWP_DryRunningSts)

#define l_u16_rd_LI0_BEWP_VoltageSts() l_u16_rd(LI0_BEWP_VoltageSts)

#define l_u16_rd_LI0_BEWP_BlockSts() l_u16_rd(LI0_BEWP_BlockSts)

#define l_u16_rd_LI0_BEWP_RESP_ERROR() l_u16_rd(LI0_BEWP_RESP_ERROR)




/* Scalar signal write */
#define l_bool_wr_LI0_BEWP_RefSpd(value) l_bool_wr(LI0_BEWP_RefSpd, value)

#define l_bool_wr_LI0_BEWP_TargetSpd(value) l_bool_wr(LI0_BEWP_TargetSpd, value)

#define l_bool_wr_LI0_BEWP_ActSpd(value) l_bool_wr(LI0_BEWP_ActSpd, value)

#define l_bool_wr_LI0_BEWP_Voltage(value) l_bool_wr(LI0_BEWP_Voltage, value)

#define l_bool_wr_LI0_BEWP_Temperature(value) l_bool_wr(LI0_BEWP_Temperature, value)

#define l_bool_wr_LI0_BEWP_Current(value) l_bool_wr(LI0_BEWP_Current, value)

#define l_bool_wr_LI0_BEWP_OverTempSts(value) l_bool_wr(LI0_BEWP_OverTempSts, value)

#define l_bool_wr_LI0_BEWP_OverCurrentSts(value) l_bool_wr(LI0_BEWP_OverCurrentSts, value)

#define l_bool_wr_LI0_BEWP_DryRunningSts(value) l_bool_wr(LI0_BEWP_DryRunningSts, value)

#define l_bool_wr_LI0_BEWP_VoltageSts(value) l_bool_wr(LI0_BEWP_VoltageSts, value)

#define l_bool_wr_LI0_BEWP_BlockSts(value) l_bool_wr(LI0_BEWP_BlockSts, value)

#define l_bool_wr_LI0_BEWP_RESP_ERROR(value) l_bool_wr(LI0_BEWP_RESP_ERROR, value)




#define l_u8_wr_LI0_BEWP_RefSpd(value) l_u8_wr(LI0_BEWP_RefSpd, value)

#define l_u8_wr_LI0_BEWP_TargetSpd(value) l_u8_wr(LI0_BEWP_TargetSpd, value)

#define l_u8_wr_LI0_BEWP_ActSpd(value) l_u8_wr(LI0_BEWP_ActSpd, value)

#define l_u8_wr_LI0_BEWP_Voltage(value) l_u8_wr(LI0_BEWP_Voltage, value)

#define l_u8_wr_LI0_BEWP_Temperature(value) l_u8_wr(LI0_BEWP_Temperature, value)

#define l_u8_wr_LI0_BEWP_Current(value) l_u8_wr(LI0_BEWP_Current, value)

#define l_u8_wr_LI0_BEWP_OverTempSts(value) l_u8_wr(LI0_BEWP_OverTempSts, value)

#define l_u8_wr_LI0_BEWP_OverCurrentSts(value) l_u8_wr(LI0_BEWP_OverCurrentSts, value)

#define l_u8_wr_LI0_BEWP_DryRunningSts(value) l_u8_wr(LI0_BEWP_DryRunningSts, value)

#define l_u8_wr_LI0_BEWP_VoltageSts(value) l_u8_wr(LI0_BEWP_VoltageSts, value)

#define l_u8_wr_LI0_BEWP_BlockSts(value) l_u8_wr(LI0_BEWP_BlockSts, value)

#define l_u8_wr_LI0_BEWP_RESP_ERROR(value) l_u8_wr(LI0_BEWP_RESP_ERROR, value)




#define l_u16_wr_LI0_BEWP_RefSpd(value) l_u16_wr(LI0_BEWP_RefSpd, value)

#define l_u16_wr_LI0_BEWP_TargetSpd(value) l_u16_wr(LI0_BEWP_TargetSpd, value)

#define l_u16_wr_LI0_BEWP_ActSpd(value) l_u16_wr(LI0_BEWP_ActSpd, value)

#define l_u16_wr_LI0_BEWP_Voltage(value) l_u16_wr(LI0_BEWP_Voltage, value)

#define l_u16_wr_LI0_BEWP_Temperature(value) l_u16_wr(LI0_BEWP_Temperature, value)

#define l_u16_wr_LI0_BEWP_Current(value) l_u16_wr(LI0_BEWP_Current, value)

#define l_u16_wr_LI0_BEWP_OverTempSts(value) l_u16_wr(LI0_BEWP_OverTempSts, value)

#define l_u16_wr_LI0_BEWP_OverCurrentSts(value) l_u16_wr(LI0_BEWP_OverCurrentSts, value)

#define l_u16_wr_LI0_BEWP_DryRunningSts(value) l_u16_wr(LI0_BEWP_DryRunningSts, value)

#define l_u16_wr_LI0_BEWP_VoltageSts(value) l_u16_wr(LI0_BEWP_VoltageSts, value)

#define l_u16_wr_LI0_BEWP_BlockSts(value) l_u16_wr(LI0_BEWP_BlockSts, value)

#define l_u16_wr_LI0_BEWP_RESP_ERROR(value) l_u16_wr(LI0_BEWP_RESP_ERROR, value)




/* Byte array read */
#define l_bytes_rd_LI0_BEWP_RefSpd(start, count, data) l_bytes_rd(LI0_BEWP_RefSpd, start, count, data)

#define l_bytes_rd_LI0_BEWP_TargetSpd(start, count, data) l_bytes_rd(LI0_BEWP_TargetSpd, start, count, data)

#define l_bytes_rd_LI0_BEWP_ActSpd(start, count, data) l_bytes_rd(LI0_BEWP_ActSpd, start, count, data)

#define l_bytes_rd_LI0_BEWP_Voltage(start, count, data) l_bytes_rd(LI0_BEWP_Voltage, start, count, data)

#define l_bytes_rd_LI0_BEWP_Temperature(start, count, data) l_bytes_rd(LI0_BEWP_Temperature, start, count, data)

#define l_bytes_rd_LI0_BEWP_Current(start, count, data) l_bytes_rd(LI0_BEWP_Current, start, count, data)

#define l_bytes_rd_LI0_BEWP_OverTempSts(start, count, data) l_bytes_rd(LI0_BEWP_OverTempSts, start, count, data)

#define l_bytes_rd_LI0_BEWP_OverCurrentSts(start, count, data) l_bytes_rd(LI0_BEWP_OverCurrentSts, start, count, data)

#define l_bytes_rd_LI0_BEWP_DryRunningSts(start, count, data) l_bytes_rd(LI0_BEWP_DryRunningSts, start, count, data)

#define l_bytes_rd_LI0_BEWP_VoltageSts(start, count, data) l_bytes_rd(LI0_BEWP_VoltageSts, start, count, data)

#define l_bytes_rd_LI0_BEWP_BlockSts(start, count, data) l_bytes_rd(LI0_BEWP_BlockSts, start, count, data)

#define l_bytes_rd_LI0_BEWP_RESP_ERROR(start, count, data) l_bytes_rd(LI0_BEWP_RESP_ERROR, start, count, data)




/* Byte array write */
#define l_bytes_wr_LI0_BEWP_RefSpd(start, count, data) l_bytes_wr(LI0_BEWP_RefSpd, start, count, data)

#define l_bytes_wr_LI0_BEWP_TargetSpd(start, count, data) l_bytes_wr(LI0_BEWP_TargetSpd, start, count, data)

#define l_bytes_wr_LI0_BEWP_ActSpd(start, count, data) l_bytes_wr(LI0_BEWP_ActSpd, start, count, data)

#define l_bytes_wr_LI0_BEWP_Voltage(start, count, data) l_bytes_wr(LI0_BEWP_Voltage, start, count, data)

#define l_bytes_wr_LI0_BEWP_Temperature(start, count, data) l_bytes_wr(LI0_BEWP_Temperature, start, count, data)

#define l_bytes_wr_LI0_BEWP_Current(start, count, data) l_bytes_wr(LI0_BEWP_Current, start, count, data)

#define l_bytes_wr_LI0_BEWP_OverTempSts(start, count, data) l_bytes_wr(LI0_BEWP_OverTempSts, start, count, data)

#define l_bytes_wr_LI0_BEWP_OverCurrentSts(start, count, data) l_bytes_wr(LI0_BEWP_OverCurrentSts, start, count, data)

#define l_bytes_wr_LI0_BEWP_DryRunningSts(start, count, data) l_bytes_wr(LI0_BEWP_DryRunningSts, start, count, data)

#define l_bytes_wr_LI0_BEWP_VoltageSts(start, count, data) l_bytes_wr(LI0_BEWP_VoltageSts, start, count, data)

#define l_bytes_wr_LI0_BEWP_BlockSts(start, count, data) l_bytes_wr(LI0_BEWP_BlockSts, start, count, data)

#define l_bytes_wr_LI0_BEWP_RESP_ERROR(start, count, data) l_bytes_wr(LI0_BEWP_RESP_ERROR, start, count, data)



/* NOTIFICATION */

#define l_flg_tst_LI0_BEWP_RefSpd_flag() l_flg_tst(LI0_BEWP_RefSpd_flag)

#define l_flg_tst_LI0_BEWP_TargetSpd_flag() l_flg_tst(LI0_BEWP_TargetSpd_flag)

#define l_flg_tst_LI0_BEWP_ActSpd_flag() l_flg_tst(LI0_BEWP_ActSpd_flag)

#define l_flg_tst_LI0_BEWP_Voltage_flag() l_flg_tst(LI0_BEWP_Voltage_flag)

#define l_flg_tst_LI0_BEWP_Temperature_flag() l_flg_tst(LI0_BEWP_Temperature_flag)

#define l_flg_tst_LI0_BEWP_Current_flag() l_flg_tst(LI0_BEWP_Current_flag)

#define l_flg_tst_LI0_BEWP_OverTempSts_flag() l_flg_tst(LI0_BEWP_OverTempSts_flag)

#define l_flg_tst_LI0_BEWP_OverCurrentSts_flag() l_flg_tst(LI0_BEWP_OverCurrentSts_flag)

#define l_flg_tst_LI0_BEWP_DryRunningSts_flag() l_flg_tst(LI0_BEWP_DryRunningSts_flag)

#define l_flg_tst_LI0_BEWP_VoltageSts_flag() l_flg_tst(LI0_BEWP_VoltageSts_flag)

#define l_flg_tst_LI0_BEWP_BlockSts_flag() l_flg_tst(LI0_BEWP_BlockSts_flag)

#define l_flg_tst_LI0_BEWP_RESP_ERROR_flag() l_flg_tst(LI0_BEWP_RESP_ERROR_flag)



#define l_flg_clr_LI0_BEWP_RefSpd_flag() l_flg_clr(LI0_BEWP_RefSpd_flag)

#define l_flg_clr_LI0_BEWP_TargetSpd_flag() l_flg_clr(LI0_BEWP_TargetSpd_flag)

#define l_flg_clr_LI0_BEWP_ActSpd_flag() l_flg_clr(LI0_BEWP_ActSpd_flag)

#define l_flg_clr_LI0_BEWP_Voltage_flag() l_flg_clr(LI0_BEWP_Voltage_flag)

#define l_flg_clr_LI0_BEWP_Temperature_flag() l_flg_clr(LI0_BEWP_Temperature_flag)

#define l_flg_clr_LI0_BEWP_Current_flag() l_flg_clr(LI0_BEWP_Current_flag)

#define l_flg_clr_LI0_BEWP_OverTempSts_flag() l_flg_clr(LI0_BEWP_OverTempSts_flag)

#define l_flg_clr_LI0_BEWP_OverCurrentSts_flag() l_flg_clr(LI0_BEWP_OverCurrentSts_flag)

#define l_flg_clr_LI0_BEWP_DryRunningSts_flag() l_flg_clr(LI0_BEWP_DryRunningSts_flag)

#define l_flg_clr_LI0_BEWP_VoltageSts_flag() l_flg_clr(LI0_BEWP_VoltageSts_flag)

#define l_flg_clr_LI0_BEWP_BlockSts_flag() l_flg_clr(LI0_BEWP_BlockSts_flag)

#define l_flg_clr_LI0_BEWP_RESP_ERROR_flag() l_flg_clr(LI0_BEWP_RESP_ERROR_flag)


/* INTERFACE MANAGEMENT */

#define l_ifc_init_LI0() l_ifc_init(LI0)



#define l_ifc_wake_up_LI0() l_ifc_wake_up(LI0)



#define l_ifc_rx_LI0() l_ifc_rx(LI0)



#define l_ifc_tx_LI0() l_ifc_tx(LI0)



#define l_ifc_aux_LI0() l_ifc_aux(LI0)



#define l_ifc_read_status_LI0() l_ifc_read_status(LI0)


#endif    /* _LIN_CFG_H_ */
