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
* @file      bsp/SCI/lin_isr.c
*
* @author    FPT Software
*
* @brief     Interrupt service routine used in LIN driver
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20101027   v1.0  First version
 * 20111005   v1.1  Updated hardware support, multi timers
 * 20111121   v1.2  Updated interrupt service routine for 9S12J637
 *
 *****************************************************************************/

#include "lin_hw_cfg.h"
#include "lin_lld_timesrv.h"
#include "lin_lld_sci.h"

#include "SDK.h"

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ISR(VectorNumber_Vsci0, SCI0_INT)
* @brief interrupt service routine for SCI0 receiver
*
* @return #void
*
* @SDD_ID N/A
* @endif
*
* @details
*   interrupt service routine for SCI0 receiver
*
* @see #lin_lld_sci_isr
*//*END*----------------------------------------------------------------------*/
//void SCI0_INT(void)
void UART1_RI_Brk_Handler(void)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_isr();
#else
  lin_lld_sci_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void TIM0_TIMER_ISR (void)
* @brief interrupt interrupt service routine for TIM timer
*
* @return #void
*
* @SDD_ID N/A
* @endif
*
* @details
*   interrupt interrupt service routine for TIM timer
*
* @see #lin_lld_timer_isr
*//*END*----------------------------------------------------------------------*/

/*************************************/
