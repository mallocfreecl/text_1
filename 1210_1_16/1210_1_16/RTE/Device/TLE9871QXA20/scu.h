/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/
/**
 * \file     scu.h
 *
 * \brief    System Control Unit low level access library
 *
 * \version  V0.3.7
 * \date     10. Sep 2019
 *
 * \note This file violates [MISRA 2012 Rule 2.2, required]
 */
 
/*******************************************************************************
**                      Author(s) Identity                                    **
********************************************************************************
**                                                                            **
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** DM           Daniel Mysliwitz                                              **
** JO           Julia Ott                                                     **
** BG           Blandine Guillot                                              **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* 
 * V0.3.7: 2019-06-05, JO:   Enable SysTick after entering Slow Mode in function SCU_EnterSlowMode
 * V0.3.6: 2018-11-28, JO:   Doxygen update, moved revision history from scu.c to scu.h
 *                           Replaced register accesses by function calls in SCU_ClkInit
 *                           Added #ifndef UNIT_TESTING_LV2 condition in SCU_EnterSleepMode
 *                           for testability 
 *                           Reworked SCU_EnterSlowMode and added a parameter for the divider
 *                           Reworked SCU_ExitSlowMode
 * V0.3.5: 2018-10-05, JO:   Parameter for prescaler to SCU_function EnterSlowMode
                             and defines for the slow down mode prescaler added;
                             SCU_ClkInit(), SCU_Init, SCU_EnterSleepMode, 
                             SCU_EnterStopMode, SCU_EnterSlowMode, SCU_ExitSlowMode, 
                             and SCU_ChangeNVMProtection rewritten
 * V0.3.4: 2018-07-06, BG:   SCU_WDT_Start(), SCU_WDT_Stop(), SCU_WDT_Service(),
 *                           SCU_OpenPASSWD(), SCU_ClosePASSWD() rewritten
 * V0.3.3: 2018-03-12, DM:   SCU_ChangeNVMProtection() return now a bool result
 * V0.3.2: 2017-10-11, DM:   MISRA 2012 compliance, the following PC-Lint rules are globally deactivated:
 *                           Info 793: ANSI/ISO limit of 6 'significant characters in an external identifier
 *                           Info 835: A zero has been given as right argument to operator
 *                           Info 845: The left argument to operator '&' is certain to be 0
 *                           The following rules are locally deactivated:
 *                           Warning 438: Last value assigned to variable 'dummy' (defined at line ...) not used
 *                           Info 838: Previously assigned value to variable 'dummy' has not been use
 *                           Replaced register accesses within functions by function calls
 *                           Replaced __STATIC_INLINE by INLINE
 * V0.3.1: 2017-05-26, DM:   global variable SystemFrequency added
 * V0.3.0: 2016-08-04, DM:   Passwd open check removed for EnterSleepMode,
 *                           EnterStopMode, EnterSlowMode
 * V0.2.9: 2015-11-26, DM:   init of DMASRCSELx added
 * V0.2.8: 2015-11-18, DM:   in function SCU_EnterStopMode() one __WFE removed
 * V0.2.7: 2015-02-10, DM:   individual header file added
 *                           OSC_CON register defined by IFXConfigWizard
 * V0.2.6: 2014-10-20, DM:   OSC_CON, OSC_PLL async. set before StopMode entry
 * V0.2.5: 2014-09-09, DM:   OSC_CON, XTAL power down handled
 * V0.2.4: 2014-06-23, TA:   conditional include of lin.h
 * V0.2.3: 2014-06-18, TA:   include bootrom.h for NVM protection control
 *                           include lin.h because of sleep functions
 * V0.2.2: 2014-06-17, DM:   fix EnterSleepMode, switch LIN Trx. into
 *                           sleep mode as well
 * V0.2.1: 2014-05-18, DM:   fix SCU_ChangeNVMProtection function
 * V0.2.0: 2014-04-26, TA:   in SCU_init(): use #defines from the header file
 *                           generated by the config wizard
 * V0.1.5: 2013-11-06, DM:   change global interrupt disable/enable in 
 *                           power saving functions
 * V0.1.4: 2013-04-05, DM:   APCLK1/2 (MI_CLK, FILT_CLK) register access
 *                           corrected
 * V0.1.3: 2013-03-26, DM:   EXINT3 removed
 * V0.1.2: 2013-03-06, DM:   NVM Protection added
 * V0.1.1: 2013-02-21, DM:   VCOSEL added to function SCU_SetSysClk()
 * V0.1.0: 2013-02-10, DM:   Initial version
 */

#ifndef SCU_H
#define SCU_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "tle987x.h"
#include "types.h"
#include "sfr_access.h"
#include "scu_defines.h"


/******************************************************************************
**                      Global Macro Definitions                             **
*******************************************************************************/
/**\brief  PASSWD Phrases, PASSWD Opened */
#define PASSWD_Open (0x98U)
/**\brief  PASSWD Phrases, PASSWD Closed */
#define PASSWD_Close (0xA8U)

/**\brief  NVM Protection indices, DATA WRITE */
#define NVM_DATA_WRITE (0U)
/**\brief  NVM Protection indices, CODE WRITE*/
#define NVM_CODE_WRITE (1U)
/**\brief  NVM Protection indices, DATA READ */
#define NVM_DATA_READ  (2U)
/**\brief  NVM Protection indices, CODE READ*/
#define NVM_CODE_READ  (3U)

/**\brief  NVM Protection actions, CLEAR */
#define PROTECTION_CLEAR  (1U)
/**\brief  NVM Protection actions, SET */
#define PROTECTION_SET    (0U)

/* Slow Down mode prescaler */
#define SLOWDOWN_PRESCALER_1     (0U)
#define SLOWDOWN_PRESCALER_2     (1U)
#define SLOWDOWN_PRESCALER_3     (2U)
#define SLOWDOWN_PRESCALER_4     (3U)
#define SLOWDOWN_PRESCALER_8     (4U)
#define SLOWDOWN_PRESCALER_16    (5U)
#define SLOWDOWN_PRESCALER_24    (6U)
#define SLOWDOWN_PRESCALER_32    (7U)
#define SLOWDOWN_PRESCALER_48    (8U)
#define SLOWDOWN_PRESCALER_64    (9U)
#define SLOWDOWN_PRESCALER_96   (10U)
#define SLOWDOWN_PRESCALER_128  (11U)
#define SLOWDOWN_PRESCALER_192  (12U)
#define SLOWDOWN_PRESCALER_256  (13U)
#define SLOWDOWN_PRESCALER_384  (14U)
#define SLOWDOWN_PRESCALER_512  (15U)


/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
/** \enum TScu_Mod
 *  \brief This enum lists the SCU Modules Configuration. 
 */
typedef enum
{
  Mod_ADC1    = 0x00, /**< SCU Modules Select: ADC1 */
  Mod_SSC1    = 0x01, /**< SCU Modules Select: SSC1 */
  Mod_CCU6    = 0x02, /**< SCU Modules Select: CCU6 */
  Mod_Timer2  = 0x03, /**< SCU Modules Select: Timer2 */
  Mod_GPT12   = 0x04, /**< SCU Modules Select: GPT12 */
  Mod_SSC2    = 0x11, /**< SCU Modules Select: SSC2 */
  Mod_Timer21 = 0x13, /**< SCU Modules Select: Timer21 */
  Mod_Timer3  = 0x15  /**< SCU Modules Select: Timer3 */
} TScu_Mod;

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
/** \brief Initializes the system clocks.
 *
 * \ingroup scu_api
 */
void SCU_ClkInit(void);

/** \brief Initializes the SCU module.
 *
 * \ingroup scu_api
 */
void SCU_Init(void);

/** \brief Sets the device into Sleep Mode
 * \note This function violates [MISRA 2012 Rule 2.2, required]
 *
 * performed steps: 
 * Set LIN to sleep mode, 
 * clear Main wake status and Wake Source MON Input,
 * stop WDT1 and trigger a short open window,
 * disable interrupts
 * set Sleep Mode Enable flag in Power Mode Control Register 0,
 * execute the WFE instruction twice to exter sleep mode
 *
 * \brief <b>Example</b><br>
 * \brief This example Sets the device into Sleep Mode.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_EnterSleepMode();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup scu_api
 */
void SCU_EnterSleepMode(void);

/** \brief Sets the device into Stop Mode
 * \note This function violates [MISRA 2012 Rule 2.2, required]
 *
 * performed steps: 
 * clear Main wake status and Wake Source MON Input,
 * stop WDT1 and trigger a short open window,
 * disable interrupts
 * select LP_CLK as sys clk for a defined state,
 * select OSC_PLL async,
 * set Sleep Mode Enable flag in Power Mode Control Register 0,
 * execute the WFE instruction twice to exter sleep mode
 * + in case TLE987X doesn't wake up with a reset:
 * reset PLL locking in PLL Control Register,
 * wait until PLL is locked,
 * switch back fSYS (was set to LP_CLK before),
 * enable interrupts,
 * initialize WDT1
 *
 * \brief <b>Example</b><br>
 * \brief This example Sets the device into Stop Mode.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_EnterStopMode();
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup scu_api
 */
void SCU_EnterStopMode(void);

/** \brief Sets the device into Slow Down Mode
 * \warning At prescalers of 384 or higher 
 * the regular Systick execution at an 1ms interval is not possible anymore.
 * The functions WDT1_Init_SlowMode needs to set the Systick interrupt
 * to a lower calling frequency, 
 * WDT1_Window_Count also needs to be adapted in this case.
 *
 * \param divider_scaled Divider from 1 up to 512 scaled to a value from 0 to 15;
 * for scaling see User manual, register CMCON1->CLKREL.
 * The macros SLOWDOWN_PRESCALER_x are recommended as parameter.
 *
 * \brief <b>Example</b><br>
 * \brief This example Sets the device into Slow Mode.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_EnterSlowMode(SLOWDOWN_PRESCALER_16);
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup scu_api
 */
void SCU_EnterSlowMode(uint8 divider_scaled);

/** \brief Gets the device out of Slow Mode
 *
 * \brief <b>Example</b><br>
 * \brief This example Gets the device out of Slow Mode.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_ExitSlowMode();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup scu_api
 */
void SCU_ExitSlowMode(void);

/** \brief Sets the Write/Read Protection for the Code/Data Flash
 *
 * \param mode Protection CODE/DATA, READ/WRITE
 * \param action CLEAR/SET
 * \retval FALSE execution passes
 * \retval TRUE execution failed, maybe wrong password
 *
 * \brief <b>Example</b><br>
 * \brief This example sets Protection for the Code Write.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   bool sts;
 *
 *   sts = SCU_ChangeNVMProtection(NVM_CODE_WRITE, PROTECTION_SET);
 * 
 *   return(sts); 
 * }
 * ~~~~~~~~~~~~~~~   
 * \ingroup scu_api
 */
bool SCU_ChangeNVMProtection(uint32 mode, uint32 action);


/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
INLINE void SCU_WDT_Start(void);
INLINE void SCU_WDT_Stop(void);
INLINE void SCU_WDT_Service(void);
INLINE void SCU_Disable_Module(TScu_Mod Module);
INLINE void SCU_Enable_Module(TScu_Mod Module);
INLINE void SCU_OpenPASSWD(void);
INLINE void SCU_ClosePASSWD(void);

/*******************************************************************************
**                      Global INLINE Function Definitions                    **
*******************************************************************************/
/** \brief Starts the Watchdog of SCU-DM in the Watchdog Timer Control Register
 * The written bit is protected by the Bit Protection Register of SCU.
 *
 * \brief <b>Example</b><br> 
 * \brief This example starts the internal Watchdog and services the internal Watchdog.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_WDT_Start();
 *   SCU_WDT_Service();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup scu_api
 */
INLINE void SCU_WDT_Start(void)
{
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->WDTCON.reg, (uint8)SCU_WDTCON_WDTEN_Pos, (uint8)SCU_WDTCON_WDTEN_Msk, 1u);
  SCU_ClosePASSWD();
}

/** \brief Stops the Watchdog of SCU-DM in the Watchdog Timer Control Register
 * The written bit is protected by the Bit Protection Register of SCU.
 *
 * \brief <b>Example</b><br> 
 * \brief This example stops the internal Watchdog.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_WDT_Stop();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup scu_api
 */
INLINE void SCU_WDT_Stop(void)
{
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->WDTCON.reg, (uint8)SCU_WDTCON_WDTEN_Pos, (uint8)SCU_WDTCON_WDTEN_Msk, 0u);
  SCU_ClosePASSWD();
}

/** \brief Services the Watchdog of SCU-DM in the Watchdog Timer Control Register
 *
 * \brief <b>Example</b><br> 
 * \brief This example starts the internal Watchdog and services the internal Watchdog.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_WDT_Start();
 *   SCU_WDT_Service();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup scu_api
 */
INLINE void SCU_WDT_Service(void)
{
  Field_Mod8(&SCU->WDTCON.reg, (uint8)SCU_WDTCON_WDTRS_Pos, (uint8)SCU_WDTCON_WDTRS_Msk, 1u);
}

/** \brief Disables a given peripheral module in the Peripheral Management Control Registers
 *
 * \param Module peripheral module (Bit[4] = PMCON1 or PMCON2, Bit[3:0] slects the bit)
 *
 * \brief <b>Example</b><br>
 * \brief This example disables ADC1 Module.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_Disable_Module(Mod_ADC1);
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup scu_api
 */
INLINE void SCU_Disable_Module(TScu_Mod Module)
{ 
  if(((uint8)Module & (uint8)0x10) == (uint8)0x10)
  {
    /* reset the corresponding bit (given by the lower nibble in the parameter Module) 
     * in register PMCON2 */
    SCU->PMCON2.reg |= (uint8) (1u << ((uint8)Module & (uint8)0x07));
  }
  else
  {
    /* reset the corresponding bit (given by the lower nibble in the parameter Module) 
     * in register PMCON1 */
    SCU->PMCON1.reg |= (uint8) (1u << ((uint8)Module & (uint8)0x07));
  }
}

/** \brief Enables a given peripheral module in the Peripheral Management Control Registers
 *
 * \param Module peripheral module (Bit[4] = PMCON1 or PMCON2, Bit[3:0] slects the bit)
 *
 * \brief <b>Example</b><br>
 * \brief This example enables ADC1 Module.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_Enable_Module(Mod_ADC1);
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup scu_api
 */
INLINE void SCU_Enable_Module(TScu_Mod Module)
{
  if(((uint8)Module & (uint8)0x10) == (uint8)0x10)
  {
    /* set the corresponding bit (given by the lower nibble in the parameter Module) 
     * in register PMCON2 */
    SCU->PMCON2.reg &= (uint8) ~ (1u << ((uint8)Module & (uint8)0x07));
  }
  else
  {
    /* set the corresponding bit (given by the lower nibble in the parameter Module) 
     * in register PMCON1 */
    SCU->PMCON1.reg &= (uint8) ~ (1u << ((uint8)Module & (uint8)0x07));
  }
}

/** \brief Opens the bit protection by writing PASSWD_Open to the Bit Protection Register
 *
 * \brief <b>Example</b><br> 
 * \brief This example opens the PASSWD Register protection scheme.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_OpenPASSWD();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup scu_api
 */
INLINE void SCU_OpenPASSWD(void)
{
  Field_Wrt8all(&SCU->PASSWD.reg, PASSWD_Open);
}

/** \brief Closes the bit protection by writing PASSWD_Close to the Bit Protection Register
 *
 * \brief <b>Example</b><br> 
 * \brief This example closes the PASSWD Register protection scheme.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   SCU_ClosePASSWD();
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup scu_api
 */
INLINE void SCU_ClosePASSWD(void)
{
	Field_Wrt8all(&SCU->PASSWD.reg, PASSWD_Close);
}

#endif
