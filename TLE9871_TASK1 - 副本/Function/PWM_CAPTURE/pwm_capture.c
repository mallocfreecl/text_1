/********************************************************************************
**         start          capture   capture                capture            **
**         Timer2         Timer2    Timer2                 Timer2             **
**       -----                --------              ---------                 **
** P1.2       |              |        |            |         |                **
**            :--------------:\       :------------          :------...       **
**            :    capDC     : \      :\                     :    capDC       **
**            :<------------>:  \     : \                    :<-----...       **
**            :                  \    :  \                   :                **
**            :     PWM_Capture() ISR :   \                  :                **
**           (captured on rising edge):    \                 :                **
**            :                       :     \                :                **
**            :    capPer             :      \               :    capPer      **
**            :<--------------------->:       \              :<-----...       **
**                                             \                              **
**                                      PWM_Capture() ISR                     ** 
**                            (captured on  falling edge)                     **
*******************************************************************************/
#include "pwm_capture.h"

extern uint32 capPer, capDC;
extern uint16 bDC_Per,overCount;
extern float DC_result,Per_result;  //串口每次只能传一个字节


/* Callback function for Timer2 */
void PWM_Capture(void)
{
  /* Timer2 in run mode */
  if (TIMER2->T2CON.bit.TR2 == 1u)            //定时器设置为下降沿启动
  {
    if (bDC_Per == 0u)                        //bDC_Per == 1表示已经捕获到上升沿
    {
      /* Get the duty cycle */
      capDC = overCount * 65536 + TIMER2_Get_Capture();
      /* Next capture on falling edge */      //改下次捕获为下降沿捕获
      TIMER2->T2MOD.bit.EDGESEL = 0u;
      bDC_Per = 1u;
    }
    else
    {
      /* Get the PWM period */
      capPer = overCount * 65536 + TIMER2_Get_Capture();      //187500Hz  1TICK = 1/187500s    24MHz的128分频
			
			//@@ After the measurement is completed, the duty cycle and period data are output @@  此时用全局变量/串口输出
			//  占空比 = capDC / capPer     x1.float = capDC / (float)capPer  x2.float = capPer / 187500.0
			DC_result = 1 - capDC / (float)capPer;  //单位%
			Per_result = capPer / 187500.0;         //单位s
			
      /* Next capture on rising edge */    //为下次捕获做准备
      TIMER2->T2MOD.bit.EDGESEL = 1u;
      bDC_Per = 0u;
      /* Stop Timer2 */
      TIMER2_Stop();
      /* Reset Timer2 */
      TIMER2_Clear_Count();
			overCount = 0;	
    }
  }
}

void PWM_Capture_Overflow(void) //定时器周期是 349520us     //可以测试一下捕获模式下是否会自动重载
{
	   overCount++;
}

