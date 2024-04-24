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
extern float DC_result,Per_result;  //����ÿ��ֻ�ܴ�һ���ֽ�


/* Callback function for Timer2 */
void PWM_Capture(void)
{
  /* Timer2 in run mode */
  if (TIMER2->T2CON.bit.TR2 == 1u)            //��ʱ������Ϊ�½�������
  {
    if (bDC_Per == 0u)                        //bDC_Per == 1��ʾ�Ѿ�����������
    {
      /* Get the duty cycle */
      capDC = overCount * 65536 + TIMER2_Get_Capture();
      /* Next capture on falling edge */      //���´β���Ϊ�½��ز���
      TIMER2->T2MOD.bit.EDGESEL = 0u;
      bDC_Per = 1u;
    }
    else
    {
      /* Get the PWM period */
      capPer = overCount * 65536 + TIMER2_Get_Capture();      //187500Hz  1TICK = 1/187500s    24MHz��128��Ƶ
			
			//@@ After the measurement is completed, the duty cycle and period data are output @@  ��ʱ��ȫ�ֱ���/�������
			//  ռ�ձ� = capDC / capPer     x1.float = capDC / (float)capPer  x2.float = capPer / 187500.0
			DC_result = 1 - capDC / (float)capPer;  //��λ%
			Per_result = capPer / 187500.0;         //��λs
			
      /* Next capture on rising edge */    //Ϊ�´β�����׼��
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

void PWM_Capture_Overflow(void) //��ʱ�������� 349520us     //���Բ���һ�²���ģʽ���Ƿ���Զ�����
{
	   overCount++;
}

