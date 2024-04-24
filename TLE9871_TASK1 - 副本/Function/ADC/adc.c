#include "adc.h"

extern uint16 Var1_mV,Var2_mV;
extern uint16 temp1;
extern uint16_t temp4; //JSCOPEʱ�����

void ADC_Init(void)
{
	
	//ʹ��ADC1����10ms�ɼ�һ�ι����ѹ
	GPT12E_T4_Start();
	
//	//ADC1   ����ģʽ��ʼ��
//	ADC1_Power_On();                      //ʹ��ADC1
//  ADC1_ANON_Set( ADC1_ANON_NORMAL );
//  ADC1_DIVA_Set( 1 );
//  ADC1_Sequencer_Mode_Sel();
//  ADC1_Sequence0_Set( ADC1_MASK_P24 );  //ͨ��4
//  
	//ADC1  ���ģʽ��ʼ��    XXXXX
	ADC1_Power_On();                     //ʹ��ADC1
  ADC1_ANON_Set( ADC1_ANON_NORMAL );   //��������ģʽ
  ADC1_DIVA_Set( 1 );                  //ʱ������
  ADC1_Software_Mode_Sel();            //ADC1����Ϊ���ģʽ

}


uint16_t ADC_Get_Value(void)
{
	uint16 temp = 0;
	ADC1_SW_Ch_Sel( ADC1_P24 );          //ͨ��4���ģʽʹ��
  ADC1_SOC_Set();                      //ADC1���ģʽ��ʼת��   �������ADת��һ��
	while(ADC1_EOC_Sts() != 1);          //�ȴ�ת�����    0:����ת�� 1:ת�����  EOC��־λ�ڶ�ȡADC DRʱ����
  temp = ADC1->RES_OUT4.bit.OUT_CH4 >> 2;
	return temp;
}
    

void gpt1_timer4_call_back(void) //����ж� 10ms
{
	temp4++;  //JSCOPEʱ�����
	
	//ȡADCת�����
	uint16 var_AD1,var_AD2;
// ����ģʽȡ����
//	ADC1_GetChResult(&var_AD1, ADC1_CH4);
//	if (ADC1_GetChResult(&var_AD1, ADC1_CH4) == true)  //ADC1ͨ��4����������ĵ�ѹ�ӿ�    //��֤����--->�ǵ�Ҫ����оƬ�ⲿ��Դ
//	{
//	  Var2_mV = (uint16)((var_AD1 * (uint16)ADC1_VREF_5000mV) / (uint16)1023);//Ĭ��Ϊ5V	
//	}

//���ģʽȡ����
	Var2_mV = (uint16)((temp1 * (uint16)ADC1_VREF_5000mV) / (uint16)1023);//Ĭ��Ϊ5V	
	

	//ADC2_VS_Result_mV();    //����֤
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
	//�ô��������������������������
}

//���Գ���(����main������)
//uint16 X,Y,test1,test2;
//  X = ADC1_GetChResult(&test1, ADC1_CH4);
//	Y = ADC1_Ch2_Result_Get();
//  if (X == true)  //ADC1ͨ��4����������ĵ�ѹ�ӿ�    //��֤����
//	{
//	  test2 = (uint16)((test1 * (uint16)ADC1_VREF_5000mV) / (uint16)1023);//Ĭ��Ϊ5V	
//	}

