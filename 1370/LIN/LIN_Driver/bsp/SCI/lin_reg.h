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
* @file      bsp/SCI/lin_reg.h
*
* @author    FPT Software
*
* @brief     SCI register definition on S08, S12, S12X
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20101027   v1.0  First version
 *
 *****************************************************************************/

#ifndef LIN_REG_H         /* Prevent duplicated includes */
#define LIN_REG_H

#include "lin_hw_cfg.h"

/*************************************/
/******   Common Register struct *****/
/*************************************/
/**
* @var typedef unsigned char tU08
* unsigned 8 bits definition
*/
typedef unsigned char   tU08;   /**< unsigned 8 bit definition */

/**
* @var typedef unsigned short tU16
* unsigned 16 bits definition
*/
typedef unsigned short    tU16;   /**< unsigned 16 bit definition */

/**
* @var typedef unsigned long tU32
* unsigned 32 bits definition
*/
typedef unsigned long   tU32;   /**< unsigned 32 bit definition */

/**
* @var typedef unsigned char tS08
* signed 32 bits definition
*/
typedef signed char     tS08;   /**< signed 8 bit definition */

/**
* @var typedef signed short      tS16
* signed 16 bits definition
*/
typedef signed short      tS16;   /**< signed 16 bit definition */

/**
* @var typedef signed long      tS32
* signed 32 bits definition
*/
typedef signed long     tS32;   /**< signed 32 bit definition */

/**
* @var uREG08 tREG08
* register 8 bits
*/

/**
* @union uREG08
* register 8 bits
*/
typedef union uREG08      /**< 8 bit register with byte and bit access*/
  {
  tU08  byte;             /**< access whole register e.g. var.byte   = 0xFF;*/
  struct
    {
    tU08 _0 :1;       /**< bit 0 */
    tU08 _1 :1;       /**< bit 1 */
  tU08 _2 :1;       /**< bit 2 */
    tU08 _3 :1;       /**< bit 3 */
    tU08 _4 :1;       /**< bit 4 */
    tU08 _5 :1;       /**< bit 5 */
    tU08 _6 :1;       /**< bit 6 */
    tU08 _7 :1;       /**< bit 7 */
    }bit;         /**< access bit at a time  e.g. var.bit._7 = 1;   */
  }tREG08;


/**
* @var uREG16 tREG16
* register 16 bits
*/

/**
* @union uREG16
* register 16 bits
*/
typedef union uREG16      /**< 16 bit register with word and byte access*/
  {
  tU16 word;              /**< access whole word */
  struct
    {
    tREG08 msb;       /**< msb byte */
    tREG08 lsb;       /**< lsb byte */
    }byte;          /**< access byte at a time */
  }tREG16;

/*************************************/
/*******   SCI Register struct *******/
/*************************************/

#define SCI_REn(enable)  (UART1->SCON.bit.REN=enable)
#define SCI_RIEn(enable) (SCU->MODIEN1.bit.RIEN1=enable)
#define SCI_TIEn(enable) (SCU->MODIEN1.bit.TIEN1=enable)
#define SCI_BreakDetect_En() (SCU->LINST.bit.BRDIS=0)  
#define SCI_BreakDetect_Dis() (SCU->LINST.bit.BRDIS=1)  
#define SCI_HwSetLowPower() (SCI_BreakDetect_Dis())

#define SCI_FrameErrDetect() ((SCI_RI() == true) && (UART1 ->SCON.bit.RB8 == 0))
#define SCI_FrameErrClr()   //(UART1->SCONCLR.bit.RB8CLR=1)
#define SCI_ByteErrDetect() (lin_lld_sci_lastbyte_tx_get() != UART1->SBUF.bit.VAL)
#define SCI_ByteErrClear()
#define SCI_Brk()       (SCU->LINST.bit.BRK      == true)
#define SCI_BrkClr()     SCU->LINSCLR.bit.BRKC   =true
#define SCI_Sync()      (SCU->LINST.bit.EOFSYN   ==true)
#define SCI_SyncClr()    SCU->LINSCLR.bit.EOFSYNC=true
#define SCI_SyncErr()   (SCU->LINST.bit.ERRSYN   == true)
#define SCI_SyncErrClr() SCU->LINSCLR.bit.ERRSYNC=true
#define SCI_TI()    (UART1->SCON.bit.TI == true)
#define SCI_TIClr()  UART1->SCONCLR.bit.TICLR = true
#define SCI_RI()    (UART1->SCON.bit.RI == true)
#define SCI_RIClr()  UART1->SCONCLR.bit.RICLR = true

#define SCIDRH_R8_MASK  0x80

#define SCI_WriteData(byte) SCI_RIClr();SCI_TIClr();UART1->SBUF.bit.VAL=byte;lin_lld_sci_lastbyte_tx_set(byte)          
#define SCI_ReadData(byte)  byte=UART1->SBUF.bit.VAL

#define SCI_SetIPR(PR) (CPU->NVIC_IPR2.bit.PRI_UART1 = ((PR)<<4))
#define SCI_GetIPR(PR) ((CPU->NVIC_IPR2.bit.PRI_UART1)>>4)

#endif /* LIN_REG_H */
/**
* @}
*/ 
