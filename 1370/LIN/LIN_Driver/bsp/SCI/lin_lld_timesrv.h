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
* @file      bsp/SCI/lin_lld_timesrv.h
*
* @author    FPT Software
*
* @brief     Timer and time interrupt for S08, S12, S12X
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20090407		v1.0	First version
 * 20111005		v1.1	Updated hardware support
 *
 *****************************************************************************/  

#ifndef __LIN_LLD_TIMESRV
#define __LIN_LLD_TIMESRV

/* MODULE LIN_LLD_TIMESRV. */
#include "lin_cfg.h" 
#include "lin_sw.h"


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_timer_S12_init()
* @brief Initialize TIM1 timer
*
*
* @SDD_ID LIN_SDD_114
* @endif
*
* @return #void
*
* @details
*   Initialize TIM1 timer
*
*//*END*----------------------------------------------------------------------*/
void lin_lld_timer_S12_init(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_timer_isr()
* @brief Timer interrupt service routine
*
* @return #void
* 
* @SDD_ID LIN_SDD_113
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*
* @see #lin_lld_sci_timeout
*
* @details
*   Timer interrupt service routine
*//*END*----------------------------------------------------------------------*/ 
#ifdef MULTI_TIMER_MODE
void lin_lld_timer_isr(l_u8 index);
#else
void lin_lld_timer_isr(void);
#endif /* End MULTI_TIMER_MODE */

#endif /* __LIN_LLD_TIMESRV */

/**
* @}
*/
