/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright $from_year$-$to_year$ Freescale Semiconductor, Inc. 
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
* @date      $date$
*  
* @brief     Hardware configuration file
* 
******************************************************************************/
$!
<lin_cfg.h file> ::= 
	<general_def> 
	<signal_flag_def> 
	<frame_flag_def>   
	<interface_handle_def>  
	<signal_handle_def>  
	<frame_handle_def>  
	<schedule_table_def>   
	<configuration_def> 
!$
#ifndef    _LIN_CFG_H_  
#define    _LIN_CFG_H_  
#include "lin_hw_cfg.h" 
/* Define operating mode */
#define _MASTER_MODE_     0 
#define _SLAVE_MODE_      1 
$if(is_master_mode)$
#define LIN_MODE   _MASTER_MODE_ 
$else$
#define LIN_MODE   _SLAVE_MODE_
$endif$

/* Define protocol version */
#define PROTOCOL_21       0  
#define PROTOCOL_J2602    1  
#define PROTOCOL_20       2 
#define LIN_PROTOCOL    $protocol_version_check$
$if(is_master_mode)$
#define LIN_NUM_OF_IFCS  $lin_number_interface$		/* For master */
$endif$
$if(is_slave_mode)$

$if(sci0_using_status)$
#define SCI_ADDR        SCI0_ADDR    /* For slave */ 
$endif$

 $if(sci1_using_status)$
 #define SCI_ADDR        SCI1_ADDR    /* For slave */
$endif$

$if(sci2_using_status)$
#define SCI_ADDR        SCI2_ADDR     /* For slave */
$endif$

$if(sci3_using_status)$
#define SCI_ADDR        SCI3_ADDR     /* For slave */
$endif$

$if(sci4_using_status)$
#define SCI_ADDR        SCI4_ADDR     /* For slave */
$endif$

$if(sci5_using_status)$
#define SCI_ADDR        SCI5_ADDR     /* For slave */
$endif$

#define LIN_BAUD_RATE    $lin_baud_rate$    	 /*For slave*/ 
$endif$

$if(is_master_mode)$
$else$
$if(resynch_support)$
/* RESYNCHRONIZE */
/*
LIN_PRESCALER = [Fbus/Baudrate/16]
RESYN_TIMER_PRESCALER >= [LIN_PRESCALER/2] (if [LIN_PRESCALER/2] > 128,please define RESYN_TIMER_PRESCALER = 128)
The value of RESYN_TIMER_PRESCALER must be one of the [ 1,2,4,8,16,32,64,128 ]) 
*/
#define LIN_PRESCALER 104
#define RESYN_TIMER_PRESCALER 64
$endif$
$endif$

/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
$if(is_gpio_using)$
#define MAX_BREAK_TIME		$max_break_time$   /* MAX_BREAK_TIME = (4000000 * 26/LIN_BAUD_RATE) */
#define MIN_BREAK_TIME    	$min_break_time$   /* MIN_BREAK_TIME = (4000000 * 12/LIN_BAUD_RATE)  */

#define _DIAG_CLASS_SUPPORT_    0

$else$
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    $diagnostic_support$
$endif$

/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      $frame_support$
$if(diagnotic_class_1)$
$endif$
$if(diagnotic_class_2)$
/*************** DIAGNOSTIC SERVICES INDEX  ************/
#define DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER       0   /**< Diagnostic service: read data by identifier */
#define DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER      1   /**< Diagnostic service: write data by identifier */
/* Diagnostic class $diagnostic_support$ */
#define _DIAG_NUMBER_OF_SERVICES_     $diag_number_of_services$           /**< Number of diagnostic services class II and class III */
$endif$
$if(is_master_mode)$
$if(diagnotic_class_3)$
/*************** DIAGNOSTIC SERVICES INDEX  ************/
#define DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER       0   /**< Diagnostic service: read data by identifier */
#define DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER      1   /**< Diagnostic service: write data by identifier */
#define DIAGSRV_SESSION_CONTROL_ORDER               2   /**< Diagnostic service: session control */
#define DIAGSRV_IO_CONTROL_BY_IDENTIFIER_ORDER      3   /**< Diagnostic service: I/O control by identifier */
#define DIAGSRV_FAULT_MEMORY_READ_ORDER             4   /**< Diagnostic service: read and clear DTC (fault memory) */
#define DIAGSRV_FAULT_MEMORY_CLEAR_ORDER            5   /**< Diagnostic service: fault memory clear */
/*Diagnostic class III */
#define _DIAG_NUMBER_OF_SERVICES_     6           /**< Number of diagnostic services class II and class III */
$endif$
$else$
$if(diagnotic_class_3)$
/*************** DIAGNOSTIC SERVICES INDEX  ************/
#define DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER       0   /**< Diagnostic service: read data by identifier */
#define DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER      1   /**< Diagnostic service: write data by identifier */
#define DIAGSRV_SESSION_CONTROL_ORDER               2   /**< Diagnostic service: session control */
#define DIAGSRV_IO_CONTROL_BY_IDENTIFIER_ORDER      3   /**< Diagnostic service: I/O control by identifier */
#define DIAGSRV_FAULT_MEMORY_READ_ORDER             4   /**< Diagnostic service: read and clear DTC (fault memory) */
#define DIAGSRV_FAULT_MEMORY_CLEAR_ORDER            5   /**< Diagnostic service: fault memory clear */
/*Diagnostic class III */
#define _DIAG_NUMBER_OF_SERVICES_     6           /**< Number of diagnostic services class II and class III */
$endif$
$endif$

/**********************************************************************/
/***************               Flags             **********************/
/**********************************************************************/
typedef unsigned int l_flag_handle;
$! [#define <interface_name>_<signal_name>_flag 0xFF<signal_flag_value: 00..FF>] !$
/* Signal flag */
$list_signal_flag_multi_value: {signal_flag | 
#define $signal_flag.("interface_name")$_$signal_flag.("signal_name")$_flag 0xFF$signal_flag.("signal_flag_value")$
}$ 

/**********Frame flag definition **********/
$! [#define  <interface_name>_<frame_name>_flag  0x<frame_flag_value: 00..FF>FF] !$
/* Frame flag */
$list_frame_flag_multi_value:{ frame_flag |
#define  $frame_flag.("interface_name")$_$frame_flag.("frame_name")$_flag  0x$frame_flag.("frame_flag_value")$FF
}$
/* Signal and Frame association */
/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
typedef enum { 
   $list_interface_handle; separator=", "$
}l_ifc_handle; 

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  $lin_number_signal$
/* List of signals */   
typedef enum {
$! <interface_name>_<signal_name> [,<interface_name>_<signal_name> ]!$
$first(list_intf_signal_handle_items): { intf_signal_handle |
   /* Interface_name = $intf_signal_handle.key$ */
 $first(intf_signal_handle.lstItem): { signal_handle |
  $intf_signal_handle.key$_$signal_handle$}$
 $rest(intf_signal_handle.lstItem): { signal_handle |
  , $intf_signal_handle.key$_$signal_handle$
 }$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
   /* Interface_name = $intf_signal_handle.key$ */
 $intf_signal_handle.lstItem: { signal_handle |
  , $intf_signal_handle.key$_$signal_handle$
 }$}$  
} l_signal_handle; 
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  $lin_number_frame$ 
/* List of frames */
typedef enum {
$! <interface_name>_<frame_name>[,<interface_name>_<frame_name>] !$
/* All frames for master node */
$first(list_intf_frame_handle_items): { intf_frame_handle |
   /* Interface_name = $intf_frame_handle.key$ */
 $first(intf_frame_handle.lstItem): { frame_handle |
  $intf_frame_handle.key$_$frame_handle$}$
 $rest(intf_frame_handle.lstItem): { frame_handle |
  , $intf_frame_handle.key$_$frame_handle$
 }$}$
$rest(list_intf_frame_handle_items): { intf_frame_handle |
   /* Interface_name = $intf_frame_handle.key$ */
 $intf_frame_handle.lstItem: { frame_handle |
  , $intf_frame_handle.key$_$frame_handle$
 }$}$  
} l_frame_handle; 
$if(is_master_mode)$
/**********************************************************************/
/***************        schedule table       *******************/
/**********************************************************************/
/* Number of schedule tables */
#define  LIN_NUM_OF_SCHD_TBL   $lin_number_of_schedule_table$ 
/* List of schedules */
typedef enum {
$! [<interface_name>_LIN_NULL_SCHEDULE, <interface_name>_GOTO_SLEEP_SCHEDULE, [<interface_name>_<schedule_table_name>],]  !$
$first(list_intf_schedule_table_handle_items): { intf_schedule_table_handle |
   /* Interface_name = $intf_schedule_table_handle.key$ */
 $first(intf_schedule_table_handle.lstItem): { schedule_table_handle |
  $intf_schedule_table_handle.key$_$schedule_table_handle$}$
 $rest(intf_schedule_table_handle.lstItem): { schedule_table_handle |
  ,$intf_schedule_table_handle.key$_$schedule_table_handle$
  }$}$
$rest(list_intf_schedule_table_handle_items): { intf_schedule_table_handle |
   /* Interface_name = $intf_schedule_table_handle.key$ */
 $intf_schedule_table_handle.lstItem: { schedule_table_handle |
  ,$intf_schedule_table_handle.key$_$schedule_table_handle$
  }$}$
}l_schedule_handle; 
$else$
$endif$ 
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
$! <configuration_def> ::= <configuration_master_mode_def> | <configuration_slave_mode_def> !$
$if(is_master_mode)$
$list_master_cfg_multi_value :{ master_cfg |
/* Size of configuration in ROM and RAM used for interface: $master_cfg.("interface_name")$ */
#define $master_cfg.("interface_name")$_LIN_SIZE_OF_CFG  $master_cfg.("lin_config_size")$
}$ 
$else$
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  $lin_config_size$ 
#define LIN_CFG_FRAME_NUM  $number_frame_except_diagnostic_frame$ 
$endif$
/**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/ 

/* SIGNAL INTERACTION */
$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
/* Scalar signal read */
#define l_bool_rd_$intf_signal_handle.key$_$signal_handle$() l_bool_rd($intf_signal_handle.key$_$signal_handle$)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bool_rd_$intf_signal_handle.key$_$signal_handle$() l_bool_rd($intf_signal_handle.key$_$signal_handle$)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_bool_rd_$intf_signal_handle.key$_$signal_handle$() l_bool_rd($intf_signal_handle.key$_$signal_handle$)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bool_rd_$intf_signal_handle.key$_$signal_handle$() l_bool_rd($intf_signal_handle.key$_$signal_handle$)
}$}$
$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_rd_$intf_signal_handle.key$_$signal_handle$() l_u8_rd($intf_signal_handle.key$_$signal_handle$)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_rd_$intf_signal_handle.key$_$signal_handle$() l_u8_rd($intf_signal_handle.key$_$signal_handle$)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_rd_$intf_signal_handle.key$_$signal_handle$() l_u8_rd($intf_signal_handle.key$_$signal_handle$)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_rd_$intf_signal_handle.key$_$signal_handle$() l_u8_rd($intf_signal_handle.key$_$signal_handle$)
}$}$

$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_rd_$intf_signal_handle.key$_$signal_handle$() l_u16_rd($intf_signal_handle.key$_$signal_handle$)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_rd_$intf_signal_handle.key$_$signal_handle$() l_u16_rd($intf_signal_handle.key$_$signal_handle$)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_rd_$intf_signal_handle.key$_$signal_handle$() l_u16_rd($intf_signal_handle.key$_$signal_handle$)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_rd_$intf_signal_handle.key$_$signal_handle$() l_u16_rd($intf_signal_handle.key$_$signal_handle$)
}$}$

$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
/* Scalar signal write */
#define l_bool_wr_$intf_signal_handle.key$_$signal_handle$(value) l_bool_wr($intf_signal_handle.key$_$signal_handle$, value)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bool_wr_$intf_signal_handle.key$_$signal_handle$(value) l_bool_wr($intf_signal_handle.key$_$signal_handle$, value)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_bool_wr_$intf_signal_handle.key$_$signal_handle$(value) l_bool_wr($intf_signal_handle.key$_$signal_handle$, value)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bool_wr_$intf_signal_handle.key$_$signal_handle$(value) l_bool_wr($intf_signal_handle.key$_$signal_handle$, value)
}$}$

$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u8_wr($intf_signal_handle.key$_$signal_handle$, value)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u8_wr($intf_signal_handle.key$_$signal_handle$, value)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u8_wr($intf_signal_handle.key$_$signal_handle$, value)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u8_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u8_wr($intf_signal_handle.key$_$signal_handle$, value)
}$}$

$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u16_wr($intf_signal_handle.key$_$signal_handle$, value)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u16_wr($intf_signal_handle.key$_$signal_handle$, value)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u16_wr($intf_signal_handle.key$_$signal_handle$, value)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_u16_wr_$intf_signal_handle.key$_$signal_handle$(value) l_u16_wr($intf_signal_handle.key$_$signal_handle$, value)
}$}$

$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
/* Byte array read */
#define l_bytes_rd_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_rd($intf_signal_handle.key$_$signal_handle$, start, count, data)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bytes_rd_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_rd($intf_signal_handle.key$_$signal_handle$, start, count, data)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_bytes_rd_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_rd($intf_signal_handle.key$_$signal_handle$, start, count, data)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bytes_rd_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_rd($intf_signal_handle.key$_$signal_handle$, start, count, data)
}$}$

$first(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
/* Byte array write */
#define l_bytes_wr_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_wr($intf_signal_handle.key$_$signal_handle$, start, count, data)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bytes_wr_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_wr($intf_signal_handle.key$_$signal_handle$, start, count, data)
}$}$
$rest(list_intf_signal_handle_items): { intf_signal_handle |
$first(intf_signal_handle.lstItem) : { signal_handle |
#define l_bytes_wr_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_wr($intf_signal_handle.key$_$signal_handle$, start, count, data)}$
$rest(intf_signal_handle.lstItem) : { signal_handle |
#define l_bytes_wr_$intf_signal_handle.key$_$signal_handle$(start, count, data) l_bytes_wr($intf_signal_handle.key$_$signal_handle$, start, count, data)
}$}$


/* NOTIFICATION */
$list_signal_flag_multi_value: {signal_flag | 
#define l_flg_tst_$signal_flag.("interface_name")$_$signal_flag.("signal_name")$_flag() l_flg_tst($signal_flag.("interface_name")$_$signal_flag.("signal_name")$_flag)
}$

$list_signal_flag_multi_value: {signal_flag | 
#define l_flg_clr_$signal_flag.("interface_name")$_$signal_flag.("signal_name")$_flag() l_flg_clr($signal_flag.("interface_name")$_$signal_flag.("signal_name")$_flag)
}$

$if(is_master_mode)$
/* SCHEDULE MANAGEMENT */
$list_interface_handle: {interface_handle |
#define l_sch_tick_$interface_handle$() l_sch_tick($interface_handle$)
}$

$list_interface_handle: {interface_handle |
#define l_sch_set_$interface_handle$(schedule, entry) l_sch_set($interface_handle$, schedule, entry)
}$
$endif$

/* INTERFACE MANAGEMENT */
$list_interface_handle: {interface_handle |
#define l_ifc_init_$interface_handle$() l_ifc_init($interface_handle$)
}$

$if(is_master_mode)$
$list_interface_handle: {interface_handle |
#define l_ifc_goto_sleep_$interface_handle$() l_ifc_goto_sleep($interface_handle$)
}$
$endif$

$list_interface_handle: {interface_handle |
#define l_ifc_wake_up_$interface_handle$() l_ifc_wake_up($interface_handle$)
}$

$list_interface_handle: {interface_handle |
#define l_ifc_rx_$interface_handle$() l_ifc_rx($interface_handle$)
}$

$list_interface_handle: {interface_handle |
#define l_ifc_tx_$interface_handle$() l_ifc_tx($interface_handle$)
}$

$list_interface_handle: {interface_handle |
#define l_ifc_aux_$interface_handle$() l_ifc_aux($interface_handle$)
}$

$list_interface_handle: {interface_handle |
#define l_ifc_read_status_$interface_handle$() l_ifc_read_status($interface_handle$)
}$

#endif    /* _LIN_CFG_H_ */

