#include "motor.h" 

uint32 pwm_cnt; //�Ӳ���static������  ������static����ʱ�޷�����
uint8 step = 1; //��ʼ����Ϊ1
uint8 dir = 1;      //0:z��ת   1:��ת    ����ת��Ҫ�ı�ÿһ���Ĳ��������ǵ����Ļ�Ϊ6��5��4��3��2��1(����)�Ϳ��Է�ת (������switch�����ж�dir)
uint8 accelerate_finish_flag;    //������ɱ�־λ
uint16 count,Cnt_JS;              //����0.5s����
uint16 Step_Time,PWM_High_Ticks;  //�������STEPTIME  PWM_HIGH_TICK �궨��


////������ٱ�   �ñ��ܷ���.h�����У���Ϊmain����
//const uint16 STEP_TIME[38] = {20000,14000,12000,10000,9000,8000,7500,7100,6800,6600,6200,6000,5800,5500,5200,4900,4600,
//	4500,4200,4000,3800,3600,3600,3500,3400,3400,3200,3000,2800,2700,2600,2500,2400,2350,2300,2150,2050,1960};
////��Ҫ��ֵ����  2400/4000 ~ 0.6
//const uint16 PWM_HIGH_TICKS[38] = {120,150,162,180,210,240,252,264,276,294,312,324,324,336,354,372,390,414,426
//,456,468,474,480,492,516,540,576,600,660,660,720,720,780,840,900,930,960,996};

void Motor_Init(void)  
{
	TIMER21_Start();      // 24M  2400TICK
	CCU6_StartTmr_T12();  //CCU6_T12_Start();
	//GPT12E_T3_Start();
	//GPT12E_T6_Start();
	
	BDRV_Init();    //ʵ�����Ѿ���main�����г�ʼ������
	
//	BDRV_Set_Channel(HS1, Ch_PWM);
//	BDRV_Set_Channel(LS1, Ch_PWM);
//	BDRV_Set_Channel(HS2, Ch_PWM);
//	BDRV_Set_Channel(LS2, Ch_PWM);
//	BDRV_Set_Channel(HS3, Ch_PWM);
//	BDRV_Set_Channel(LS3, Ch_PWM);
	
	//HS1��LS1��HS2��LS2 ʹ��ͨ��������PWM���ģʽ��ص���CTRL1�Ĵ���
	BDRV->CTRL1.bit.HS1_EN = 1u;
	BDRV->CTRL1.bit.HS1_PWM = 1u;
	BDRV->CTRL1.bit.LS1_EN = 1u;
	BDRV->CTRL1.bit.LS1_PWM = 1u;
	
	BDRV->CTRL1.bit.HS2_EN = 1u;
	BDRV->CTRL1.bit.HS2_PWM = 1u;
	BDRV->CTRL1.bit.LS2_EN = 1u;
	BDRV->CTRL1.bit.LS2_PWM = 1u;
	
	BDRV->CTRL2.bit.HS3_EN = 1u;
	BDRV->CTRL2.bit.HS3_PWM = 1u;
	BDRV->CTRL2.bit.LS3_EN = 1u;
	BDRV->CTRL2.bit.LS3_PWM = 1u;
	
	BDRV->CTRL3.bit.DSMONVTH = 4u;  //��ɱ������ѹ��ֵ���
}


void timer21_call_back(void) //100us
{
//	if(accelerate_finish_flag == 0)//��δ����
//	{
//		count++;
//		if(5000 == count)  //5000*100us == 500000us == 500ms == 0.5s
//		{
//			//����ÿ����ʱ��(us)��ռ�ձ��иߵ�ƽ��TICKS����(һ��PWM����2400��TICK,24M,100us)~ʵ���Ǿ��Ǹı�ռ�ձȣ��ı���ߵ�ѹ 
//			//Cnt_JS : 0~37
//			Step_Time = STEP_TIME[Cnt_JS];
//			PWM_High_Ticks = PWM_HIGH_TICKS[Cnt_JS];
//			Cnt_JS++;	
//			count = 0;
//			if(38 == Cnt_JS ) accelerate_finish_flag = 1;//�������  //Cnt_JS����Ҫ���㣬��Ϊ����Ҫѭ��  16s��ʹ�ü��ٱ�[37]���ٶ�
//		}
//	}
	
	
	//��ת:1��3��5���½���2��4��6��������
	pwm_cnt++;
	                                  //(STEPTIME/100) ���ܳ���uint32_t�ķ�Χ // ��Ƶ����XHz,һ���Ƕ���΢��  X������С��
	if(pwm_cnt <= (uint32_t)(STEPTIME/100) )   //������ÿһ����N��pwm����      // ((X/6.0) * (1000000))/100 == (X*10000)/6.0 ~ (uint16_t) ((X*10000)/6.0)  //ֱ�Ӱ�ÿ������΢��Ƚ�ֱ��
	{
		if(dir)
		{
			switch(step)
			{
				case 1: 
				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;       //���ڱȽ�ֵ�ߵ�ƽ CC60
				CCU6->CC61SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);//100.0��Ŀ����ռ��תΪ�����ͣ����ȸ��ߣ�����������������ȥС��
				CCU6->CC62SR.reg = (uint16)0x00;
				break;
				
				case 2: 
				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;
				CCU6->CC61SR.reg = (uint16)0x00; 
				CCU6->CC62SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
				break;
				
				case 3: 
				CCU6->CC60SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
				CCU6->CC61SR.reg = (uint16)0x00;
				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK; 
				break;
				
				case 4: 
				CCU6->CC60SR.reg = (uint16)0x00;
				CCU6->CC61SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK; 
				break;
				
				case 5: 
				CCU6->CC60SR.reg = (uint16)0x00;
				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;
				CCU6->CC62SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
				break;
			
				case 6: 
				CCU6->CC60SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;
				CCU6->CC62SR.reg = (uint16)0x00;
				break;
			}	
		}
//		else
//		{
//			switch(step)
//			{
//				case 1: 
//				CCU6->CC60SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK); 
//				CCU6->CC61SR.reg = (uint16)0x00;
//				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:ʮ����60   //���ڱȽ�ֵ�ߵ�ƽ CC60
//				break;
//				
//				case 2: 
//				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;
//				CCU6->CC61SR.reg = (uint16)0x00; 
//				CCU6->CC62SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				break;
//				
//				case 3: 
//				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;
//				CCU6->CC61SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
//				CCU6->CC62SR.reg = (uint16)0x00; 
//				break;
//				
//				case 4: 
//				CCU6->CC60SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;
//				CCU6->CC62SR.reg = (uint16)0x00; 
//				break;
//				
//				case 5: 
//				CCU6->CC60SR.reg = (uint16)0x00;
//				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;
//				CCU6->CC62SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
//				break;
//			
//				case 6: 
//				CCU6->CC60SR.reg = (uint16)0x00;
//				CCU6->CC61SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK;
//				break;
//			}
//		}
			
			CCU6_T12_Str_En();    //���رȽ�ֵ
			CCU6_T12_Str_En();    //���رȽ�ֵ
	}
	else     //����һ��100us��������ر��������ıȽ�ֵ
	{
		if(step == 6) step = 0;
		step++;	
		pwm_cnt = 0;
	}

}


//�Ĳ�����TIMER12�����ԣ�TIMER12��24Mʱ�ӱȽϾ�ȷ�Ҳ���Ҫ���ö�ʱ����ֵ
//��ÿ���ж�����ı� ��һ����ռ�ձȹ̶� ��ͨ���Ƚ�ֵ
//��gpt1_timer3���ж��иı�  ��һ����ռ�ձ���Ҫ�ı� ��ͨ���Ƚ�ֵ 

// 1����1/6s,1��PWM������1/10000s(10kHz),1������(1/6) / (1/10000) == 10000/6 ��PWM���� Լ����1667������   
//@@@@@@��Ϊ1664�����ڱȽϾ�ȷ       ���õ���6MHz-16��Ƶ ��104������       
// gpt1_timer3��ʱ��ʱ��Ӧ����(PWM����100us)���������� XXX     

//void gpt1_timer3_call_back(void) //100us   �ı�Ƚ�ֵ  //CC60:�����෴�����ڱȽ�ֵΪ�ߵ�ƽ�����ڱȽ�ֵΪ�͵�ƽ  ����PWM����600��TICK(6MHz)         
//{
//	//1��3��5��������2��4��6���½���
//	pwm_cnt++;
//	                                  //(STEPTIME/100.0) ���ܳ���uint16_t�ķ�Χ // ��Ƶ����XHz,һ���Ƕ���΢��  X������С��
//	if(pwm_cnt < (uint16_t)(STEPTIME/100.0) )   //������ÿһ����N��pwm����      // ((X/6.0) * (1000000))/100 == (X*10000)/6.0 ~ (uint16_t) ((X*10000)/6.0)  //ֱ�Ӱ�ÿ������΢��Ƚ�ֱ��
//	{
//			switch(step)
//			{
//				case 1: 
//				CCU6->CC60SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK); 
//				CCU6->CC61SR.reg = (uint16)0x00;
//				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:ʮ����60   //���ڱȽ�ֵ�ߵ�ƽ CC60
//				break;
//				
//				case 2: 
//				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:ʮ����60 
//				CCU6->CC61SR.reg = (uint16)0x00; 
//				CCU6->CC62SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				break;
//				
//				case 3: 
//				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:ʮ����60
//				CCU6->CC61SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
//				CCU6->CC62SR.reg = (uint16)0x00; 
//				break;
//				
//				case 4: 
//				CCU6->CC60SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:ʮ����60
//				CCU6->CC62SR.reg = (uint16)0x00; 
//				break;
//				
//				case 5: 
//				CCU6->CC60SR.reg = (uint16)0x00;
//				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:ʮ����60 
//				CCU6->CC62SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
//				break;
//			
//				case 6: 
//				CCU6->CC60SR.reg = (uint16)0x00;
//				CCU6->CC61SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:ʮ����60
//				break;
//			}
//			CCU6_T12_Str_En();    //���رȽ�ֵ
//	}
//	else     //����һ����������ر��������ıȽ�ֵ
//	{
//	  if(step == 6) step = 0;
//		step++;
//		pwm_cnt = 0;
//	}
//	GPT12E->T3.reg = 64936;//65536-600
//}


//void gpt2_timer6_call_back(void) //  6M��128��Ƶ  ��62400��(0.1664s,Լ1/6s)�ж�һ��
//{
//	step++;
//	switch(step)
//	{
//		case 1: 
//		CCU6->CC61SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x3c;//0x3c:ʮ����60   //���ڱȽ�ֵ�ߵ�ƽ CC60
//		break;
//		
//		case 2: 
//		CCU6->CC60SR.reg = (uint16)0x3c;//0x3c:ʮ����60 
//		CCU6->CC61SR.reg = (uint16)0x00; 
//		break;
//		
//		case 3: 
//		CCU6->CC60SR.reg = (uint16)0x3c;//0x3c:ʮ����60
//		CCU6->CC62SR.reg = (uint16)0x00; 
//		break;
//		
//		case 4: 
//		CCU6->CC61SR.reg = (uint16)0x3c;//0x3c:ʮ����60
//		CCU6->CC62SR.reg = (uint16)0x00; 
//		break;
//		
//		case 5: 
//		CCU6->CC60SR.reg = (uint16)0x00;
//		CCU6->CC61SR.reg = (uint16)0x3c;//0x3c:ʮ����60 
//		break;
//	
//		case 6: 
//		CCU6->CC60SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x3c;//0x3c:ʮ����60
//		break;
//	
//	}
//	CCU6_T12_Str_En();    //���رȽ�ֵ
//	if(step == 6) step = 0;
//	
//	GPT12E->T6.reg = 3136;//65536-62400
//}


////��ÿ���ж�����ı� ��һ����ռ�ձȹ̶� ��ͨ���Ƚ�ֵ
////��gpt1_timer3���ж��иı�  ��һ����ռ�ձ���Ҫ�ı� ��ͨ���Ƚ�ֵ 

//// 1����1/6s,1��PWM������1/10000s(10kHz),1������(1/6) / (1/10000) == 10000/6 ��PWM���� Լ����1667������   
////@@@@@@��Ϊ1664�����ڱȽϾ�ȷ       ���õ���6MHz-16��Ƶ ��104������       
//// gpt1_timer3��ʱ��ʱ��Ӧ����(PWM����100us)����������                                                                                
//void gpt1_timer3_call_back(void) //100us   �ı�Ƚ�ֵ  //CC60:�����෴�����ڱȽ�ֵΪ�ߵ�ƽ�����ڱȽ�ֵΪ�͵�ƽ  ����PWM����600��TICK(6MHz)         
//{
//	//1��3��5��������2��4��6���½���
//	time1++;
//	
//	if(time1 % 104 == 0)   //��Ϊ16С����
//	{
//			switch(step)
//			{
//				case 1: 
//				CCU6->CC60SR.reg = (uint16)(time1/1664.0 * 90);   
//				break;
//				
//				case 2: 
//				CCU6->CC62SR.reg = (uint16)((1- time1/1664.0) * 90);
//				break;
//				
//				case 3: 
//				CCU6->CC61SR.reg = (uint16)(time1/1664.0 * 90);
//				break;
//				
//				case 4: 
//				CCU6->CC60SR.reg = (uint16)((1- time1/1664.0) * 90);
//				break;
//				
//				case 5: 
//				CCU6->CC62SR.reg = (uint16)(time1/1664.0 * 90);
//				break;
//			
//				case 6: 
//				CCU6->CC61SR.reg = (uint16)((1- time1/1664.0) * 90);
//				break;
//			}
//	}
//	
//	CCU6_T12_Str_En();    //���رȽ�ֵ
//	
//	if(time1 == 1664) time1 = 0;
//	GPT12E->T3.reg = 64936;//65536-600
//}



//�������
//void gpt1_timer3_call_back(void) //33.33us   �ı�Ƚ�ֵ       //�����෴�����ڱȽ�ֵΪ�ߵ�ƽ�����ڱȽ�ֵΪ�͵�ƽ  ����PWM����600��TICK
//{
//	temp3++;
//	
//	time1++;
//	if(time1 == 1) //�ڶ���1/3����Ӧ�����Ĳ���
//	{
//		CCU6->CC60SR.reg = (uint16)0x00; 
//		CCU6->CC61SR.reg = (uint16)0x3c;//0x3c:ʮ����60 
//		CCU6->CC62SR.reg = (uint16)0x00;
//		CCU6_T12_Str_En(); 
//	}
//	else if(time1 == 2)
//	{
//		CCU6->CC60SR.reg = (uint16)0x00; 
//		CCU6->CC61SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x3c;//0x3c:ʮ����60 
//		CCU6_T12_Str_En(); 
//	}
//	else if(time1 == 3)
//	{
//		CCU6->CC60SR.reg = (uint16)0x3c;//0x3c:ʮ����60
//		CCU6->CC61SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x00; 
//		CCU6_T12_Str_En(); 
//		time1 = 0; 	
//	}
//	GPT12E->T3.reg = 65336;
//}


