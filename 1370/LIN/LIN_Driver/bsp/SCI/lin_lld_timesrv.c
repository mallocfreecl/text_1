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
* @file      bsp/SCI/lin_lld_timesrv.c
*
* @author    FPT Software
*
* @brief     Timer for S08, S12, S12X
*
******************************************************************************/  

/******************************************************************************
 *
 * History:
 *
 * 20101027		v1.0	First version
 * 20111005		v1.1	Updated hardware support
 * 20111121   v1.2  Updated timer config for J637
 *
 *****************************************************************************/
 
#include "lin_lld_timesrv.h"
#include "lin_lld_sci.h"
#include "lin_common_proto.h"
#include "lin_hw_cfg.h"

#if (LIN_MODE == _MASTER_MODE_)
	extern const lin_hardware_name lin_virtual_ifc[LIN_NUM_OF_IFCS];
#endif /* End (LIN_MODE == _MASTER_MODE_) */

#ifdef MULTI_TIMER_MODE
	extern const l_u16 time_base_period[LIN_NUM_OF_IFCS];
#endif /* End MULTI_TIMER_MODE */


void lin_lld_timer_S12_init(void) 
{
/* Init Timer there */
	
}

#ifdef MULTI_TIMER_MODE
   void lin_lld_timer_isr(l_u8 index)
   {
      /* Timeout for SCI channel */    
      lin_lld_sci_timeout(lin_virtual_ifc[index]);    
      /*** Master task firing ***/  
   }
#else /* Else MULTI_TIMER_MODE */
   void lin_lld_timer_isr(void)
   {
   #if LIN_MODE == _MASTER_MODE_
     l_u8 i;
     /* Timeout for SCI channel */    
     for(i=0; i<LIN_NUM_OF_IFCS; i++) { 
       lin_lld_sci_timeout(lin_virtual_ifc[i]);
     }    
     /*** Master task firing ***/  
   #else     
     lin_lld_sci_timeout();
   #endif /* End LIN_MODE == _MASTER_MODE_ */
   }
#endif /* End MULTI_TIMER_MODE */

/**
* @}
*/
