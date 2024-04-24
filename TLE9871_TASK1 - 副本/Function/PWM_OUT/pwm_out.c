#include "pwm_out.h"


extern uint16 time;
//extern uint16_t temp2;

//拉高1s，拉低2s  //已验证
void PWM_Out(void)
{
	GPT12E_T2_Start();
	
}
//GPT1_timer2_frequency = 0.01M

//不需要手动生成更新事件，令影子寄存器发挥作用
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


void gpt1_timer2_call_back(void) // 溢出时间为1s   拉低1S，放开2s    有三极管的存在，电平需要置反   P1.4就需要高1s,低2s   P1.4需要上拉输入(检测PWM输入时直接关闭定时器就可以)
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



