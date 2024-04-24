#include "tle_device.h"
#include "eval_board.h"
#include "motor.h" 
#include "adc.h"
#include "pwm_out.h"
#include "my_uart.h"

//PWM输出的计时标志位
uint16 time;   

//PWM输入捕获变量
uint32 capPer, capDC;
uint16 bDC_Per,overCount;    //全局变量bDC_Per未初始化为0  当捕获到下降沿bDC_Per由0转1，捕获结束清为0
float DC_result,Per_result;  //串口每次只能传一个字节

//ADC变量
uint16 Var1_mV,Var2_mV;
uint16 temp1;                //使用软件模式AD值暂存变量

//串口变量
uint8 ch;                    //串口发送字节变量

//JSCOPE变量
uint16_t temp2,temp3,temp4;  //用JSCOPE查看GPT定时时间是否准确

 
int main(void)
{
  /* Initialization of hardware modules based on Config Wizard configuration */
  TLE_Init();
	
//	//拉高1s，拉低2s  //已验证
//	PWM_Out();
	
	//PWM捕获不需开启初始化   已在TLE_Init();配置了使能   捕获PWM时要注释掉PWM_Out(); (GPT12E_T2_Start();)
	//bDC_Per = 0;  //加不加都可以

  //三相PWM输出
	Motor_Init();
	
	//串口初始化相关程序  //配置文件初始化中可能已经做了
//	UART2_Receiver_En();  //UART2串口接收使能
//	UART2_BaudRate_Set(19200);
	
	//ADC的初始化
  //ADC_Init(); //里面包含了GPT12E_T4 的开启
  
  for (;;)
  {		
		temp1 = ADC_Get_Value();  //主循环中不断获取最新ADC1通道4的AD值
   (void)WDT1_Service();
  }
}


//待电流稳定后采集电流
void CSA_CurGet(void)
{
	uint16 CSA_Offset = CSA_Offset_Get() >> 2;
	uint16 CSA_Var = ADC1_Ch1_Result_Get() >> 2;
	uint16 CSA_Cur = ((CSA_Var - CSA_Offset) * (5 / 1023.0) - (0.4 * 5)) / 40 * 1000 * 1000;  //电阻是1m欧   增益设为40   CSA_Cur的单位是mA
	//UART2_Send_Byte((uint8_t)CSA_Cur);
	Serial_Printf("此时的电流为:%d",CSA_Cur);//也可以将CSA_Cur放大1000倍(更精确)，输出在串口
  //Serial_SendString((char *)(&CSA_Cur));
}
