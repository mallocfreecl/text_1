#include "adc.h"

extern uint16 Var1_mV,Var2_mV;
extern uint16 temp1;
extern uint16_t temp4; //JSCOPE时间测试

void ADC_Init(void)
{
	
	//使用ADC1外设10ms采集一次供电电压
	GPT12E_T4_Start();
	
//	//ADC1   序列模式初始化
//	ADC1_Power_On();                      //使能ADC1
//  ADC1_ANON_Set( ADC1_ANON_NORMAL );
//  ADC1_DIVA_Set( 1 );
//  ADC1_Sequencer_Mode_Sel();
//  ADC1_Sequence0_Set( ADC1_MASK_P24 );  //通道4
//  
	//ADC1  软件模式初始化    XXXXX
	ADC1_Power_On();                     //使能ADC1
  ADC1_ANON_Set( ADC1_ANON_NORMAL );   //设置正常模式
  ADC1_DIVA_Set( 1 );                  //时钟设置
  ADC1_Software_Mode_Sel();            //ADC1设置为软件模式

}


uint16_t ADC_Get_Value(void)
{
	uint16 temp = 0;
	ADC1_SW_Ch_Sel( ADC1_P24 );          //通道4软件模式使能
  ADC1_SOC_Set();                      //ADC1软件模式开始转换   软件触发AD转换一次
	while(ADC1_EOC_Sts() != 1);          //等待转换完成    0:正在转换 1:转换完成  EOC标志位在读取ADC DR时清零
  temp = ADC1->RES_OUT4.bit.OUT_CH4 >> 2;
	return temp;
}
    

void gpt1_timer4_call_back(void) //溢出中断 10ms
{
	temp4++;  //JSCOPE时间测试
	
	//取ADC转换结果
	uint16 var_AD1,var_AD2;
// 序列模式取数据
//	ADC1_GetChResult(&var_AD1, ADC1_CH4);
//	if (ADC1_GetChResult(&var_AD1, ADC1_CH4) == true)  //ADC1通道4是热敏电阻的电压接口    //验证不了--->记得要开启芯片外部电源
//	{
//	  Var2_mV = (uint16)((var_AD1 * (uint16)ADC1_VREF_5000mV) / (uint16)1023);//默认为5V	
//	}

//软件模式取数据
	Var2_mV = (uint16)((temp1 * (uint16)ADC1_VREF_5000mV) / (uint16)1023);//默认为5V	
	

	//ADC2_VS_Result_mV();    //已验证
  var_AD2 = ADC2_GetChResult(ADC2_VS);
  if (ADC2_VS_Attenuator_Range_Get() == ADC2_VS_Attenuator_Range_3_28V)
  {
    /* Conversion for VS attenuator range up to 28 V */
    Var1_mV = (uint16)(((var_AD2 * (uint32)((uint16)ADC2_VREF_mV * (uint16)VS_ATTEN_CONV_NOMIN_MAX28V)) / (uint16)ADC2_FILTOUT_MAX) / (uint8)VS_ATTEN_CONV_DENOM_MAX28V);
  }
  else
  {
    /* Conversion for VS attenuator range up to 22 V */
    Var1_mV = (uint16)(((var_AD2 * (uint32)((uint16)ADC2_VREF_mV * (uint16)VS_ATTEN_CONV_NOMIN_MAX22V)) / (uint16)ADC2_FILTOUT_MAX) / (uint8)VS_ATTEN_CONV_DENOM_MAX22V);
  }
	GPT12E->T4.reg = 65067;
	//用串口输出结果，或在主函数中输出
}

//测试程序(放在main函数中)
//uint16 X,Y,test1,test2;
//  X = ADC1_GetChResult(&test1, ADC1_CH4);
//	Y = ADC1_Ch2_Result_Get();
//  if (X == true)  //ADC1通道4是热敏电阻的电压接口    //验证不了
//	{
//	  test2 = (uint16)((test1 * (uint16)ADC1_VREF_5000mV) / (uint16)1023);//默认为5V	
//	}

