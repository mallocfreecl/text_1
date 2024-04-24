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

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "ccu6.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/
/** \brief Initializes the CCU6 module.
 *
 * \param None
 * \return None
 *
 * \ingroup drv_api
 */
void CCU6_Init(void)
{
  CCU6->TCTR0.reg = (uint16) CCU6_TCTR0;    //寄存器TCTR0控制定时器T12和T13的基本功能
  CCU6->T12PR.reg = (uint16) CCU6_T12PR;    //预分频器
  CCU6->T13PR.reg = (uint16) CCU6_T13PR;
  CCU6->T12DTC.reg = (uint16) CCU6_T12DTC;  //设置死区时间，三个通道的死区时间一致
  CCU6->CC60SR.reg = (uint16) CCU6_CC60SR;  //CC60SR是比较/捕获影子寄存器，可读可写  CC60S是比较/捕获寄存器，只可读 
  CCU6->CC61SR.reg = (uint16) CCU6_CC61SR;
  CCU6->CC62SR.reg = (uint16) CCU6_CC62SR;
  CCU6->CC63SR.reg = (uint16) CCU6_CC63SR;
  CCU6->TCTR2.reg = (uint16) CCU6_TCTR2;    //寄存器TCTR2控制定时器T12和T13的单次功能和同步功能。
  CCU6->TRPCTR.reg = (uint16) CCU6_TRPCTR;  //寄存器TRPCTR控制trap功能
  CCU6->MODCTR.reg = (uint16) CCU6_MODCTR;  //寄存器MODCTR包含控制位，可以通过定时器T12和T13产生的PWM模式调制相应的输出信号。
  CCU6->MCMCTR.reg = (uint16) CCU6_MCMCTR;  //寄存器MCMCTR包含用于多通道功能的控制位
  CCU6->T12MSEL.reg = (uint16) CCU6_T12MSEL;//Capture/Compare T12 Mode Select Register 比较/捕获模式选择
  CCU6->PSLR.reg = (uint16) CCU6_PSLR;      //寄存器PSLR定义由模块输出引脚驱动的无源状态电平。Passive State Level Register
  CCU6->INP.reg = (uint16) CCU6_INP;        //寄存器INP包含中断节点指针，允许灵活的中断处理。
  CCU6->IEN.reg = (uint16) CCU6_IEN;        //寄存器IEN包含中断使能位和控制位，用于在错误霍尔模式的情况下启用自动空闲功能。
  CCU6->CMPSTAT.reg = (uint16) CCU6_CMPSTAT;//比较状态寄存器CMPSTAT包含监控当前捕获和比较状态的状态位，以及定义比较通道的主动/被动状态的控制位
  CCU6->PISEL0.reg = (uint16) CCU6_PISEL0;  //寄存器PISEL0和PISEL2包含为模块输入选择实际输入端口/信号的位字段。
  CCU6->PISEL2.reg = (uint16) CCU6_PISEL2;
  CCU6_T12_Str_En();                        //使能影子传输
  CCU6_T13_Str_En();
}
