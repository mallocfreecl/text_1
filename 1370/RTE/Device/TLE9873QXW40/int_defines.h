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
#ifndef INT_DEFINES_H
#define INT_DEFINES_H

/* XML Version 2.0.6 */
#define INT_XML_VERSION (20006)

#define CPU_BUSFAULT_EN (0x0) /*decimal 0*/

#define CPU_HARDFAULT_EN (0x0) /*decimal 0*/

#define CPU_MEMMANAGE_EN (0x0) /*decimal 0*/

#define CPU_NVIC_IPR0 (0xF0F010) /*decimal 15790096*/

#define CPU_NVIC_IPR1 (0xF0F000F0) /*decimal 4042260720*/

#define CPU_NVIC_IPR2 (0xF030F0F0) /*decimal 4029739248*/

#define CPU_NVIC_IPR3 (0xF0F0F0F0) /*decimal 4042322160*/

#define CPU_NVIC_ISER0 (0xC21) /*decimal 3105*/

#define CPU_SHPR3 (0x20000000) /*decimal 536870912*/

#define CPU_SYSTICK_EN (0x1) /*decimal 1*/

#define CPU_USAGEFAULT_EN (0x0) /*decimal 0*/

#define PMU_VDDC_OL_EN (0x0) /*decimal 0*/

#define PMU_VDDC_OV_EN (0x0) /*decimal 0*/

#define PMU_VDDP_OL_EN (0x0) /*decimal 0*/

#define PMU_VDDP_OV_EN (0x0) /*decimal 0*/

#define SCUPM_BDRV_IRQ_CTRL (0x3FC00) /*decimal 261120*/

#define SCUPM_SYS_IRQ_CTRL (0x0) /*decimal 0*/

#define SCUPM_SYS_SUPPLY_IRQ_CTRL (0x0) /*decimal 0*/

#define SCU_DMAIEN1 (0x0) /*decimal 0*/

#define SCU_DMAIEN2 (0x0) /*decimal 0*/

#define SCU_EDCCON (0x0) /*decimal 0*/

#define SCU_EXICON0 (0xFF) /*decimal 255*/

#define SCU_GPT12IEN (0x5) /*decimal 5*/

#define SCU_MODIEN1 (0x0) /*decimal 0*/

#define SCU_MODIEN2 (0xC0) /*decimal 192*/

#define SCU_NMICON (0x0) /*decimal 0*/

#endif /* INT_DEFINES_H */
