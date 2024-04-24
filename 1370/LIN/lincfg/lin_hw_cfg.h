/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2019-2020 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
****************************************************************************//*! 
* 
* @file      lin_hw_cfg.h 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Tue Jun 16 15:17:58 CST 2020
*  
* @brief     Hardware configuration file
* 
******************************************************************************/
#ifndef      _LIN_HW_CFG_H
#define      _LIN_HW_CFG_H
#include "TLE987x.h" 
#include "uart_defines.h"
#include "scu_defines.h"
#ifndef _MC9S12VR48_H
#define _MC9S12VR48_H
#endif
typedef enum { 
   SCI0, 
   SCI1, 
   SCI2, 
   SCI3, 
   SCI4, 
   SCI5, 
   GPIO, 
   SLIC 
} lin_hardware_name;

/* MCU type definition */      
#define _S08_   0 
#define _S12_   1 
#define _S12X_  2 

/* Type of MCU */
#define _MCU_  _S12_

#define SCI_V2  0
#define SCI_V4  1
#define SCI_V5  2

/* SCI version */
#define SCI_VERSION SCI_V5

/* Resynchronization support */
#define __RESYN_EN   0
 
/* Interface type that MCU uses */
#define XGATE_SUPPORT 0
#define _LIN_XGATE_   0 
#define _LIN_SCI_     1 
#define _LIN_SLIC_    0
#define _LIN_GPIO_    0
/***********  SCI HARDWARE SECTION  *********/
#define NUM_OF_SCI_CHANNEL    1
/* SCI Base Register definition */
#define SCI0_ADDR  0x00C8  
#define SCI1_ADDR  0x00D0  
 
/* Use SCI Channel  */ 
#define _SCI0_   1 
#define _SCI1_   0 
#define _SCI2_   0 
#define _SCI3_   0 
#define _SCI4_   0 
#define _SCI5_   0 
/* MCU bus frequency */
#define MCU_BUS_FREQ       25000000

#define TIME_BASE_PERIOD   1000
/* max idle timeout for all networks = idle_timeout_value*1000000/time_base_period */
#define _MAX_IDLE_TIMEOUT_    4000      /* idle_timeout_value = 5s */
#endif
