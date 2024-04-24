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
 * \file     csa.h
 *
 * \brief    Current Sense Amplifier low level access library
 *
 * \version  V0.1.4
 * \date     27. Nov 2018
 */

/*******************************************************************************
**                      Author(s) Identity                                    **
********************************************************************************
**                                                                            **
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** DM           Daniel Mysliwitz                                              **
** BG           Blandine Guillot                                              **
** JO           Julia Ott                                                     **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 * V0.1.4: 2018-11-27, JO:   Doxygen update, moved revision history from csa.c to csa.h
 * V0.1.3: 2018-07-07, BG:   CSA_Set_Gain(uint8 gain) function rewritten
 * V0.1.2: 2017-09-29, DM:   CSA_Offset_Get() function added
 *                           MISRA 2012 compliance, the following PC-Lint rules are globally deactivated:
 *                           Info 793: ANSI/ISO limit of 6 'significant characters in an external identifier
 *                           Info 835: A zero has been given as right argument to operator
 *                           Info 845: The left argument to operator '&' is certain to be 0
 *                           Replaced register accesses within functions by function calls
 *                           Replaced __STATIC_INLINE by INLINE
 * V0.1.1: 2015-02-10, DM:   individual header file added
 * V0.1.0: 2014-06-02, DM:   Initial version
 */

#ifndef CSA_H
#define CSA_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "tle987x.h"
#include "types.h"
#include "sfr_access.h"

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
/** \enum TCSA_Gain
 *  \brief This enum lists the CSA Gain Settings. 
 */
typedef enum
{
  CSA_GAIN_10  = 0, /**< Operational Amplifier Gain Factor 10 */
  CSA_GAIN_20  = 1, /**< Operational Amplifier Gain Factor 20 */
  CSA_GAIN_40  = 2, /**< Operational Amplifier Gain Factor 40 */
  CSA_GAIN_60  = 3  /**< Operational Amplifier Gain Factor 60 */
} TCSA_Gain;

/*******************************************************************************
**                        Global Function Declarations                           **
*******************************************************************************/
/** \brief switches on CSA module.
 *
 * \brief <b>Example</b><br>
 * \brief This example switches on CSA module.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   CSA_Power_On(); 
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup csa_api
 */
INLINE void CSA_Power_On(void)
{
	Field_Mod32(&CSA->CTRL.reg, MF_CSA_CTRL_EN_Pos, MF_CSA_CTRL_EN_Msk, 1u);
}

/** \brief switches off CSA module.
 *
 * \brief <b>Example</b><br>
 * \brief This example switches off CSA module.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   CSA_Power_Off(); 
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup csa_api
 */
INLINE void CSA_Power_Off(void)
{
	Field_Mod32(&CSA->CTRL.reg, MF_CSA_CTRL_EN_Pos, MF_CSA_CTRL_EN_Msk, 0u);
}

/** \brief Set CSA Gain value
 *
 * \param gain Gain setting
 *
 * \brief <b>Example</b><br>
 * \brief This example configures CSA Gain value to 40. 
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   CSA_Gain_Set(2u); 
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup csa_api
 */
INLINE void CSA_Gain_Set(uint32 gain)
{
	Field_Mod32(&CSA->CTRL.reg, MF_CSA_CTRL_GAIN_Pos, MF_CSA_CTRL_GAIN_Msk, (gain));
}

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
/** \brief Initializes the CSA module.
 *
 * \ingroup csa_api
 */
void CSA_Init(void);

INLINE void CSA_Set_Gain(uint8 gain);

/** \brief measures the CSA offset using ADC1.
 * \brief this measurement must be done with 0A through the shunt
 * 
 * \return CSA offset
 *
 * \ingroup csa_api
 */
uint16 CSA_Offset_Get(void);

/*******************************************************************************
**                      Global INLINE Function Definitions                    **
*******************************************************************************/
/** \brief Set CSA Gain value
 *
 * \param gain Gain setting
 *
 * \brief <b>Example</b><br>
 * \brief This example configures CSA Gain value to 40. 
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   CSA_Set_Gain(2u); 
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup adc2_api
 */
INLINE void CSA_Set_Gain(uint8 gain)
{
	Field_Mod32(&CSA->CTRL.reg, MF_CSA_CTRL_GAIN_Pos, MF_CSA_CTRL_GAIN_Msk, gain);
}


#endif
