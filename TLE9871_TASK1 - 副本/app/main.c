#include "tle_device.h"
#include "eval_board.h"
#include "motor.h" 
#include "adc.h"
#include "pwm_out.h"
#include "my_uart.h"

//PWM����ļ�ʱ��־λ
uint16 time;   

//PWM���벶�����
uint32 capPer, capDC;
uint16 bDC_Per,overCount;    //ȫ�ֱ���bDC_Perδ��ʼ��Ϊ0  �������½���bDC_Per��0ת1�����������Ϊ0
float DC_result,Per_result;  //����ÿ��ֻ�ܴ�һ���ֽ�

//ADC����
uint16 Var1_mV,Var2_mV;
uint16 temp1;                //ʹ�����ģʽADֵ�ݴ����

//���ڱ���
uint8 ch;                    //���ڷ����ֽڱ���

//JSCOPE����
uint16_t temp2,temp3,temp4;  //��JSCOPE�鿴GPT��ʱʱ���Ƿ�׼ȷ

 
int main(void)
{
  /* Initialization of hardware modules based on Config Wizard configuration */
  TLE_Init();
	
//	//����1s������2s  //����֤
//	PWM_Out();
	
	//PWM�����迪����ʼ��   ����TLE_Init();������ʹ��   ����PWMʱҪע�͵�PWM_Out(); (GPT12E_T2_Start();)
	//bDC_Per = 0;  //�Ӳ��Ӷ�����

  //����PWM���
	Motor_Init();
	
	//���ڳ�ʼ����س���  //�����ļ���ʼ���п����Ѿ�����
//	UART2_Receiver_En();  //UART2���ڽ���ʹ��
//	UART2_BaudRate_Set(19200);
	
	//ADC�ĳ�ʼ��
  //ADC_Init(); //���������GPT12E_T4 �Ŀ���
  
  for (;;)
  {		
		temp1 = ADC_Get_Value();  //��ѭ���в��ϻ�ȡ����ADC1ͨ��4��ADֵ
   (void)WDT1_Service();
  }
}


//�������ȶ���ɼ�����
void CSA_CurGet(void)
{
	uint16 CSA_Offset = CSA_Offset_Get() >> 2;
	uint16 CSA_Var = ADC1_Ch1_Result_Get() >> 2;
	uint16 CSA_Cur = ((CSA_Var - CSA_Offset) * (5 / 1023.0) - (0.4 * 5)) / 40 * 1000 * 1000;  //������1mŷ   ������Ϊ40   CSA_Cur�ĵ�λ��mA
	//UART2_Send_Byte((uint8_t)CSA_Cur);
	Serial_Printf("��ʱ�ĵ���Ϊ:%d",CSA_Cur);//Ҳ���Խ�CSA_Cur�Ŵ�1000��(����ȷ)������ڴ���
  //Serial_SendString((char *)(&CSA_Cur));
}
