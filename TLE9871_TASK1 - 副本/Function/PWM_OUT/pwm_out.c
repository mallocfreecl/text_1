#include "pwm_out.h"


extern uint16 time;
//extern uint16_t temp2;

//����1s������2s  //����֤
void PWM_Out(void)
{
	GPT12E_T2_Start();
	
}
//GPT1_timer2_frequency = 0.01M

//����Ҫ�ֶ����ɸ����¼�����Ӱ�ӼĴ�����������
//void gpt1_timer2_call_back(void)
//{
//	if(PORT_P00_Get() == 0)
//	{		
//		PORT_P00_Output_High_Set();//PORT->P0_DATA.reg |= 0x01;   PORT_P00_Output_Toggle_Set();  
//		GPT12E_T2_Value_Set(45536);//GPT12E->T2.reg = 45536;
//	}
//	else
//	{
//		PORT_P00_Output_Low_Set();//PORT_P00_Output_Toggle_Set();
//		GPT12E_T2_Value_Set(55536);//GPT12E->T2.reg = 55536;
//	}
//}

//Whether the GPT reloads automatically?  NO


void gpt1_timer2_call_back(void) // ���ʱ��Ϊ1s   ����1S���ſ�2s    �������ܵĴ��ڣ���ƽ��Ҫ�÷�   P1.4����Ҫ��1s,��2s   P1.4��Ҫ��������(���PWM����ʱֱ�ӹرն�ʱ���Ϳ���)
{
	//temp2++;
	
	time++;
	if(time == 1)
		PORT_P14_Output_Low_Set();	
	if(time == 3)
	{
		PORT_P14_Output_High_Set();
		time = 0;
	}
	GPT12E->T2.reg = 18661;
}



