/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/** @addtogroup common_core_api_group
* @{ */
/**************************************************************************//**
*
* @file      lin_common_api.h
*
* @author    FPT Software
*
* @brief     Common LIN API functions
*
******************************************************************************/ 
/******************************************************************************
 *
 * History:
 *
 * 20090407		v1.0	First version
 *
 *****************************************************************************/
 
/** @} */

#ifndef _LIN_COMMON_API_H
#define _LIN_COMMON_API_H

#include "lin_sw.h"
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_sys_init ( void )
* @brief This function performs the initialization of the LIN core
*
* @return #l_bool
* 
* @SDD_ID LIN_SDD_238
* @endif
*
* @details
*   This function performs the initialization of the LIN core
*
* @see  #lin_lld_setup_xgate
* @see  #lin_lld_timer_init
*
* @addtogroup driver_cluster_group
*//*END*----------------------------------------------------------------------*/
l_bool        l_sys_init (void);
/** @addtogroup signal_interract_group
* @{
*/
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_bool_rd (l_signal_handle sss)
* @brief This function reads and returns the current value of the signal
*
* @param sss <B>[IN]</B> signal name
*
* @return #l_u8
*     <BR>0 if successful, else return error code
* 
* @SDD_ID LIN_SDD_239
* @endif
*
* @static_global_var
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function reads and returns the current value of the signal
*//*END*----------------------------------------------------------------------*/
l_bool        l_bool_rd (l_signal_handle sss);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 l_u8_rd (l_signal_handle sss)
* @brief This function reads and returns the current value of the signal
*
* @param sss <B>[IN]</B> signal name
*
* @return #l_u8
* 
* @SDD_ID LIN_SDD_240
* @endif
*
* @static_global_var
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function reads and returns the current value of the signal
*//*END*----------------------------------------------------------------------*/
l_u8          l_u8_rd (l_signal_handle sss);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u16 l_u16_rd (l_signal_handle sss)
* @brief This function reads and returns the current value of the signal
*
* @param sss <B>[IN]</B> signal name 
*
* @return #l_u16
* 
* @SDD_ID LIN_SDD_241
* @endif
*
* @static_global_var
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function reads and returns the current value of the signal
*
*//*END*----------------------------------------------------------------------*/
l_u16         l_u16_rd (l_signal_handle sss);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_bool_wr (l_signal_handle sss, l_bool v)
* @brief This function sets the new value for the current signal
*
* @param sss <B>[IN]</B> signal name
* @param v <B>[IN]</B> input value
*
* @return #void
* 
* @SDD_ID LIN_SDD_243
* @endif
*
* @local_var
*   -# <B>#l_ifc_handle</B> <I>iii</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function sets the new value for the current signal
*
* @see  #lin_find_associate_ifc
* @see  #lin_update_frame_flag
*//*END*----------------------------------------------------------------------*/
void          l_bool_wr (l_signal_handle sss, l_bool v);    
void          l_bool_wr_b (l_signal_handle sss, l_bool v, l_u8 frm_index); 
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_u8_wr (l_signal_handle sss, l_u8 v)
* @brief This function sets the new value for the current signal
*
* @param sss <B>[IN]</B> signal name
* @param v <B>[IN]</B> input value
*
* @return #void
* 
* @SDD_ID LIN_SDD_244
* @endif
*
* @local_var
*   -# <B>#l_ifc_handle</B> <I>iii</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function sets the new value for the current signal
*
* @see  #lin_find_associate_ifc
* @see  #lin_update_frame_flag
*//*END*----------------------------------------------------------------------*/
void          l_u8_wr (l_signal_handle sss, l_u8 v );
void          l_u8_wr_b (l_signal_handle sss, l_u8 v, l_u8 frm_index);
void          l_u8_index_wr (l_u8 index);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_u16_wr (l_signal_handle sss, l_u16 v)
* @brief sets the new value for the current signal
*
* @param sss <B>[IN]</B> signal name
* @param v <B>[IN]</B> input value
*
* @return #void
* 
* @SDD_ID LIN_SDD_245
* @endif
*
* @local_var
*   -# <B>#l_ifc_handle</B> <I>iii</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function sets the new value for the current signal
*
* @see  #lin_find_associate_ifc
* @see  #lin_update_frame_flag
*//*END*----------------------------------------------------------------------*/
void          l_u16_wr (l_signal_handle sss, l_u16 v);
void          l_u16_wr_b (l_signal_handle sss, l_u16 v, l_u8 frm_index);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_bytes_rd (l_signal_handle sss, l_u8 start, l_u8 count, l_u8* const data)
* @brief reads and returns the current values of the selected bytes in the signal
*
* @param sss <B>[IN]</B> signal name
* @param start <B>[IN]</B> first byte to read from
* @param count <B>[IN]</B> number of bytes to read
* @param data <B>[OUT]</B> output data
*
* @return #void
* 
* @SDD_ID LIN_SDD_242
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>j</I>
*
* @static_global_var
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function reads and returns the current values of the selected bytes in the signal
*
*//*END*----------------------------------------------------------------------*/
void          l_bytes_rd (l_signal_handle sss, l_u8 start, l_u8 count, l_u8* const data);


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_bytes_wr (l_signal_handle sss, l_u8 start, l_u8 count, l_u8* const data)
* @brief sets the new value for the current signal
*
* @param sss <B>[IN]</B> signal name
* @param start <B>[IN]</B> first byte to write from 
* @param count <B>[IN]</B> number of bytes to write
* @param data <B>[IN]</B> data will be read from
*
* @return #void
* 
* @SDD_ID LIN_SDD_246
* @endif
*
* @local_var
*   -# <B>#l_ifc_handle</B> <I>iii</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>j</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function sets the new value for the current signal
*
* @see  #lin_find_associate_ifc
* @see  #lin_update_frame_flag
*//*END*----------------------------------------------------------------------*/
void          l_bytes_wr (l_signal_handle sss, l_u8 start, l_u8 count, l_u8* const data);
void          l_bytes_wr_b (l_signal_handle sss, l_u8 start, l_u8 count, l_u8* const data, l_u8 frm_index);
/** @} */

/** @addtogroup notification_group 
* @{ */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_flg_tst (l_flag_handle fff)
* @brief returns the current state of the flag
*
* @param fff <B>[IN]</B> flag name 
*
* @return #l_bool
* 
* @SDD_ID LIN_SDD_247
* @endif
*
* @static_global_var
*   -# <B>#lin_flag_frame_handle_tbl</B>
*   -# <B>#lin_flag_signal_handle_tbl</B>
*
* @details
*   This function returns the current state of the flag specified by name 
*   i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* @see  #lin_find_associate_ifc
* @see  #lin_update_frame_flag
*//*END*----------------------------------------------------------------------*/
l_bool        l_flg_tst (l_flag_handle fff); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_flg_clr (l_flag_handle fff)
* @brief sets the current value of the flag specified by name to zero
*
* @param fff <B>[IN]</B> flag name
*
* @return #void
* 
* @SDD_ID LIN_SDD_248
* @endif
*
* @local_var
*   -# <B>#l_ifc_handle</B> <I>iii</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>j</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*   -# <B>#lin_signal_tbl</B>
*
* @details
*   This function sets the current value of the flag specified by name to zero
*
*//*END*----------------------------------------------------------------------*/
void          l_flg_clr (l_flag_handle fff);  
/** @} */
#if LIN_MODE == _MASTER_MODE_
/** @addtogroup schedule_management_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_sch_set (l_ifc_handle iii, l_schedule_handle schedule_iii, l_u8 entry)
* @brief Set up the next schedule to be followed by the l_sch_tick
*
* @param iii <B>[IN]</B> interface name 
* @param schedule_iii <B>[IN]</B> schedule table for interface
* @param entry <B>[IN]</B> entry to be set
*
* @return #void
* 
* @SDD_ID LIN_SDD_249
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Set up the next schedule to be followed by the l_sch_tick 
*   function for a certain interface. The new schedule will be 
*   activated as soon as the current schedule reaches 
*   its next schedule entry point
*
*//*END*----------------------------------------------------------------------*/
void          l_sch_set (l_ifc_handle iii, l_schedule_handle schedule_iii, l_u8 entry); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u8 l_sch_tick (l_ifc_handle iii)
* @brief  follows a schedule
*
* @param iii <B>[IN]</B> interface name 
*
* @return #l_u8
* 
* @SDD_ID LIN_SDD_250
* @endif
*
* @details
*   This function follows a schedule. When a frame becomes due, its 
*   transmission is initiated. When the end of the current schedule is reached, 
*   this function starts again at the beginning of the schedule.
*
* @see #lin_tick_callback_handler
*//*END*----------------------------------------------------------------------*/
l_u8          l_sch_tick (l_ifc_handle iii);
/** @} */

/** @addtogroup interface_management_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_goto_sleep (l_ifc_handle iii)
* @brief Request slave nodes  to enter bus sleep mode
*
* @param iii <B>[IN]</B> interface name 
*
* @return #void
* 
* @SDD_ID LIN_SDD_252
* @endif
*
* @local_var
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Request slave nodes on the cluster connected to the interface to enter 
*   bus sleep mode by issuing one go to sleep command.
*
* @see  #l_sch_set
*//*END*----------------------------------------------------------------------*/
void          l_ifc_goto_sleep (l_ifc_handle iii);
/** @} */
#endif /* LIN_MODE == _MASTER_MODE_ */
/** @addtogroup interface_management_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_bool l_ifc_init (l_ifc_handle iii)
* @brief Initialize the controller specified by name
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_bool
* 
* @SDD_ID LIN_SDD_251
* @endif
*
* @details
*   Initialize the controller specified by name, i.e. sets up internal 
*   functions such as the baud rate. The default schedule set by the l_ifc_init 
*   call will be the L_NULL_SCHEDULE where no frames will be sent and received.
*   This is the first call a user must perform, before using any other interface 
*   related LIN API functions.
*   The function returns zero if the initialization was successful and non-zero if failed.
*
* @see  #lin_lld_init
*//*END*----------------------------------------------------------------------*/
l_bool        l_ifc_init (l_ifc_handle iii);
/* l_ifc_ioctl  */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_wake_up (l_ifc_handle iii)
* @brief Transmit the wake up signal. 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_253
* @endif
*
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Transmit the wake up signal. 
*
* @see  #lin_lld_tx_wake_up
*//*END*----------------------------------------------------------------------*/
void          l_ifc_wake_up (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_rx (l_ifc_handle iii)
* @brief Receive one frame 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_254
* @endif
*
* @details
*   Receive one frame 
*
*//*END*----------------------------------------------------------------------*/
void          l_ifc_rx (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_tx (l_ifc_handle iii)
* @brief Transmit one frame 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_255
* @endif
*
* @details
*   Transmit one frame 
*
*//*END*----------------------------------------------------------------------*/
void          l_ifc_tx (l_ifc_handle iii); 

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u16 l_ifc_read_status (l_ifc_handle iii)
* @brief This function will return the status of the previous communication 
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_u16
* 
* @SDD_ID LIN_SDD_259
* @endif
*
* @local_var
*   -# <B>#l_u16</B> <I>tmp_word_status</I>
*   -# <B>#lin_configuration</B> <I>conf</I>
*
* @static_global_var
*   -# <B>#lin_word_status</B>
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   This function will return the status of the previous communication 
*
*//*END*----------------------------------------------------------------------*/
l_u16         l_ifc_read_status (l_ifc_handle iii);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_ifc_aux (l_ifc_handle iii)
* @brief synchronize to the break/sync field
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_256
* @endif
*
* @details
*   This function may be used in the slave nodes to synchronize to the 
*   break/sync field sequence transmitted by the master node on the 
*   interface specified by name. 
*
*//*END*----------------------------------------------------------------------*/

void          l_ifc_aux (l_ifc_handle iii);
/** @} */

/** @addtogroup call_out_group 
* @{ */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_u16 l_sys_irq_disable (l_ifc_handle iii)
* @brief Disable LIN related IRQ 
*
* @param iii <B>[IN]</B> interface name
*
* @return #l_u16
* 
* @SDD_ID LIN_SDD_257
* @endif
*
* @details
*   Disable LIN related IRQ 
*
* @see #lin_lld_int_disable
*//*END*----------------------------------------------------------------------*/
l_u16         l_sys_irq_disable (l_ifc_handle iii);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void l_sys_irq_restore (l_ifc_handle iii)
* @brief Enable LIN related IRQ 
*
* @param iii <B>[IN]</B> interface name
*
* @return #void
* 
* @SDD_ID LIN_SDD_258
* @endif
*
* @details
*   Enable LIN related IRQ 
*
* @see #lin_lld_int_enable
*//*END*----------------------------------------------------------------------*/
void          l_sys_irq_restore (l_ifc_handle iii);
/** @} */
/** @addtogroup common_core_api_group
* @{ */
#if LIN_MODE == _SLAVE_MODE_

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_frame_flag (l_signal_handle sss)
* @brief Update associate frame flag with a signal
*
* @param sss <B>[IN]</B> signal name
*
* @return #void
* 
* @SDD_ID LIN_SDD_281
* @endif
*
* @local_var
*   -# <B>#lin_frm2sig</B> <I>*ptr</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_s8</B> <I>j</I>
*
* @static_global_var
*   -# <B>#lin_signal_flag_tbl</B>
*   -# <B>#lin_frame_tbl</B>
*   -# <B>#lin_frame_flag_tbl</B>
*
* @details
*   Update associate frame flag with a signal
*
*//*END*----------------------------------------------------------------------*/
void          lin_update_frame_flag(l_signal_handle sss,l_u8 frm_index);
#endif
#if LIN_MODE == _MASTER_MODE_
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_update_frame_flag (lin_configuration *conf_ptr, l_signal_handle sss)
* @brief Update associate frame flag with a signal
*
* @param conf_ptr <B>[IN]</B> LIN configuration
* @param sss <B>[IN]</B> signal name
*
* @return #void
* 
* @SDD_ID LIN_SDD_262
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_s8</B> <I>j</I>
*   -# <B>#l_s8</B> <I>k</I>
*   -# <B>#lin_frm2sig</B> <I>*ptr</I>
*
* @static_global_var
*   -# <B>#lin_signal_flag_tbl</B>
*   -# <B>#lin_frame_tbl</B>
*   -# <B>#lin_frame_flag_tbl</B>
*
* @details
*   Update associate frame flag with a signal
*
*//*END*----------------------------------------------------------------------*/
void          lin_update_frame_flag(lin_configuration *conf_ptr, l_signal_handle sss);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn l_ifc_handle lin_find_associate_ifc (l_signal_handle sss)
* @brief Find the associated interface with a signal 
*
* @param sss <B>[IN]</B> signal name
*
* @return #l_ifc_handle
* 
* @SDD_ID LIN_SDD_263
* @endif
*
* @local_var
*   -# <B>#l_ifc_handle</B> <I>found</I>
*   -# <B>#l_u8</B> <I>i</I>
*   -# <B>#l_u8</B> <I>signal_start</I>
*
* @static_global_var
*   -# <B>#lin_ifc_configuration</B>
*
* @details
*   Find the associated interface with a signal 
*
*//*END*----------------------------------------------------------------------*/
l_ifc_handle  lin_find_associate_ifc (l_signal_handle sss);
#endif /* End LIN_MODE == _MASTER_MODE_ */
/** @} */
#endif  /* _LIN_COMMON_API_H */
