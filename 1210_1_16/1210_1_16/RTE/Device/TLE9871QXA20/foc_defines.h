/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2018, Infineon Technologies AG
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
#ifndef FOC_DEFINES_H
#define FOC_DEFINES_H

/* XML Version 2.0.8 */
#define FOC_XML_VERSION (20008)

#define FOC_CUR_ADJUST (0.50)

#define FOC_END_START_SPEED (0x258) /*decimal 600*/

#define FOC_En (0x1) /*decimal 1*/

#define FOC_FLUX_ADJUST (0.05)

#define FOC_L_PHASE (0.00006)

#define FOC_MAX_CUR_SPEED (800.00000)

#define FOC_MAX_NEG_REF_CUR (-27.00000)

#define FOC_MAX_POS_REF_CUR (27.00000)

#define FOC_MAX_SPEED (0x1F40) /*decimal 8000*/

#define FOC_MIN_CUR_SPEED (-800.00000)

#define FOC_MIN_NEG_REF_CUR (-10.00000)

#define FOC_MIN_POS_REF_CUR (10.00000)

#define FOC_NOM_CUR (20.00000)

#define FOC_POLE_PAIRS (0x2) /*decimal 2*/

#define FOC_PWM_FREQ (0x4E20) /*decimal 20000*/

#define FOC_PWM_PERIOD (0x32) /*decimal 50*/

#define FOC_R_PHASE (0.03)

#define FOC_R_SHUNT (0.001)

#define FOC_SPEED_FILT_TIME (0.02)

#define FOC_SPEED_KI (0xC8) /*decimal 200*/

#define FOC_SPEED_KP (0x96) /*decimal 150*/

#define FOC_START_ACCEL (300.00000)

#define FOC_START_CUR (0x3) /*decimal 3*/

#define FOC_START_CUR_IF (0x3) /*decimal 3*/

#define FOC_START_FREQ_ZERO (0x1) /*decimal 1*/

#define FOC_SWITCH_ON_SPEED (0x14) /*decimal 20*/

#define FOC_ZERO_VEC_TIME (0.10)

#endif /* FOC_DEFINES_H */
