#include "motor.h" 

uint32 pwm_cnt; //加不加static都可以  但加了static仿真时无法访问
uint8 step = 1; //初始步数为1
uint8 dir = 1;      //0:z正转   1:反转    正反转需要改变每一步的操作，不是单纯的换为6、5、4、3、2、1(逆序)就可以反转 (可以在switch加入判断dir)
uint8 accelerate_finish_flag;    //加速完成标志位
uint16 count,Cnt_JS;              //用于0.5s计数
uint16 Step_Time,PWM_High_Ticks;  //起初是用STEPTIME  PWM_HIGH_TICK 宏定义


////定义加速表   该表不能放在.h函数中，因为main函数
//const uint16 STEP_TIME[38] = {20000,14000,12000,10000,9000,8000,7500,7100,6800,6600,6200,6000,5800,5500,5200,4900,4600,
//	4500,4200,4000,3800,3600,3600,3500,3400,3400,3200,3000,2800,2700,2600,2500,2400,2350,2300,2150,2050,1960};
////需要改值，乘  2400/4000 ~ 0.6
//const uint16 PWM_HIGH_TICKS[38] = {120,150,162,180,210,240,252,264,276,294,312,324,324,336,354,372,390,414,426
//,456,468,474,480,492,516,540,576,600,660,660,720,720,780,840,900,930,960,996};

void Motor_Init(void)  
{
	TIMER21_Start();      // 24M  2400TICK
	CCU6_StartTmr_T12();  //CCU6_T12_Start();
	//GPT12E_T3_Start();
	//GPT12E_T6_Start();
	
	BDRV_Init();    //实际上已经在main函数中初始化过了
	
//	BDRV_Set_Channel(HS1, Ch_PWM);
//	BDRV_Set_Channel(LS1, Ch_PWM);
//	BDRV_Set_Channel(HS2, Ch_PWM);
//	BDRV_Set_Channel(LS2, Ch_PWM);
//	BDRV_Set_Channel(HS3, Ch_PWM);
//	BDRV_Set_Channel(LS3, Ch_PWM);
	
	//HS1、LS1、HS2、LS2 使能通道与设置PWM输出模式相关的是CTRL1寄存器
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
	
	BDRV->CTRL3.bit.DSMONVTH = 4u;  //电荷泵输出电压幅值相关
}


void timer21_call_back(void) //100us
{
//	if(accelerate_finish_flag == 0)//还未加速
//	{
//		count++;
//		if(5000 == count)  //5000*100us == 500000us == 500ms == 0.5s
//		{
//			//减短每步的时间(us)，占空比中高电平的TICKS增大(一个PWM波共2400个TICK,24M,100us)~实际是就是改变占空比，改变最高电压 
//			//Cnt_JS : 0~37
//			Step_Time = STEP_TIME[Cnt_JS];
//			PWM_High_Ticks = PWM_HIGH_TICKS[Cnt_JS];
//			Cnt_JS++;	
//			count = 0;
//			if(38 == Cnt_JS ) accelerate_finish_flag = 1;//加速完成  //Cnt_JS不需要清零，因为不需要循环  16s后都使用加速表[37]的速度
//		}
//	}
	
	
	//正转:1，3，5步下降。2，4，6步上升。
	pwm_cnt++;
	                                  //(STEPTIME/100) 不能超过uint32_t的范围 // 电频率是XHz,一步是多少微秒  X可以是小数
	if(pwm_cnt <= (uint32_t)(STEPTIME/100) )   //六步中每一步有N个pwm波形      // ((X/6.0) * (1000000))/100 == (X*10000)/6.0 ~ (uint16_t) ((X*10000)/6.0)  //直接按每步多少微秒比较直接
	{
		if(dir)
		{
			switch(step)
			{
				case 1: 
				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;       //低于比较值高电平 CC60
				CCU6->CC61SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);//100.0的目的是占比转为浮点型，精度更高，不会像整数那样舍去小数
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
//				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:十进制60   //低于比较值高电平 CC60
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
			
			CCU6_T12_Str_En();    //加载比较值
			CCU6_T12_Str_En();    //加载比较值
	}
	else     //单独一个100us用来改相关变量，不改比较值
	{
		if(step == 6) step = 0;
		step++;	
		pwm_cnt = 0;
	}

}


//改不改用TIMER12都可以，TIMER12是24M时钟比较精确且不需要重置定时器的值
//在每步中断里面改变 在一步中占空比固定 的通道比较值
//在gpt1_timer3的中断中改变  在一步中占空比需要改变 的通道比较值 

// 1步是1/6s,1个PWM周期是1/10000s(10kHz),1步就是(1/6) / (1/10000) == 10000/6 个PWM周期 约等于1667个周期   
//@@@@@@化为1664个周期比较精确       正好等于6MHz-16分频 的104个周期       
// gpt1_timer3定时的时间应该是(PWM周期100us)的整数倍， XXX     

//void gpt1_timer3_call_back(void) //100us   改变比较值  //CC60:极性相反，低于比较值为高电平，高于比较值为低电平  整个PWM周期600个TICK(6MHz)         
//{
//	//1，3，5步上升。2，4，6步下降。
//	pwm_cnt++;
//	                                  //(STEPTIME/100.0) 不能超过uint16_t的范围 // 电频率是XHz,一步是多少微秒  X可以是小数
//	if(pwm_cnt < (uint16_t)(STEPTIME/100.0) )   //六步中每一步有N个pwm波形      // ((X/6.0) * (1000000))/100 == (X*10000)/6.0 ~ (uint16_t) ((X*10000)/6.0)  //直接按每步多少微秒比较直接
//	{
//			switch(step)
//			{
//				case 1: 
//				CCU6->CC60SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK); 
//				CCU6->CC61SR.reg = (uint16)0x00;
//				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:十进制60   //低于比较值高电平 CC60
//				break;
//				
//				case 2: 
//				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:十进制60 
//				CCU6->CC61SR.reg = (uint16)0x00; 
//				CCU6->CC62SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				break;
//				
//				case 3: 
//				CCU6->CC60SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:十进制60
//				CCU6->CC61SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
//				CCU6->CC62SR.reg = (uint16)0x00; 
//				break;
//				
//				case 4: 
//				CCU6->CC60SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:十进制60
//				CCU6->CC62SR.reg = (uint16)0x00; 
//				break;
//				
//				case 5: 
//				CCU6->CC60SR.reg = (uint16)0x00;
//				CCU6->CC61SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:十进制60 
//				CCU6->CC62SR.reg = (uint16)((pwm_cnt/(STEPTIME/100.0)) * PWM_HIGH_TICK);
//				break;
//			
//				case 6: 
//				CCU6->CC60SR.reg = (uint16)0x00;
//				CCU6->CC61SR.reg = (uint16)((1- (pwm_cnt/(STEPTIME/100.0))) * PWM_HIGH_TICK);
//				CCU6->CC62SR.reg = (uint16)PWM_HIGH_TICK;//0x3c:十进制60
//				break;
//			}
//			CCU6_T12_Str_En();    //加载比较值
//	}
//	else     //单独一个用来改相关变量，不改比较值
//	{
//	  if(step == 6) step = 0;
//		step++;
//		pwm_cnt = 0;
//	}
//	GPT12E->T3.reg = 64936;//65536-600
//}


//void gpt2_timer6_call_back(void) //  6M的128分频  记62400次(0.1664s,约1/6s)中断一次
//{
//	step++;
//	switch(step)
//	{
//		case 1: 
//		CCU6->CC61SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x3c;//0x3c:十进制60   //低于比较值高电平 CC60
//		break;
//		
//		case 2: 
//		CCU6->CC60SR.reg = (uint16)0x3c;//0x3c:十进制60 
//		CCU6->CC61SR.reg = (uint16)0x00; 
//		break;
//		
//		case 3: 
//		CCU6->CC60SR.reg = (uint16)0x3c;//0x3c:十进制60
//		CCU6->CC62SR.reg = (uint16)0x00; 
//		break;
//		
//		case 4: 
//		CCU6->CC61SR.reg = (uint16)0x3c;//0x3c:十进制60
//		CCU6->CC62SR.reg = (uint16)0x00; 
//		break;
//		
//		case 5: 
//		CCU6->CC60SR.reg = (uint16)0x00;
//		CCU6->CC61SR.reg = (uint16)0x3c;//0x3c:十进制60 
//		break;
//	
//		case 6: 
//		CCU6->CC60SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x3c;//0x3c:十进制60
//		break;
//	
//	}
//	CCU6_T12_Str_En();    //加载比较值
//	if(step == 6) step = 0;
//	
//	GPT12E->T6.reg = 3136;//65536-62400
//}


////在每步中断里面改变 在一步中占空比固定 的通道比较值
////在gpt1_timer3的中断中改变  在一步中占空比需要改变 的通道比较值 

//// 1步是1/6s,1个PWM周期是1/10000s(10kHz),1步就是(1/6) / (1/10000) == 10000/6 个PWM周期 约等于1667个周期   
////@@@@@@化为1664个周期比较精确       正好等于6MHz-16分频 的104个周期       
//// gpt1_timer3定时的时间应该是(PWM周期100us)的整数倍，                                                                                
//void gpt1_timer3_call_back(void) //100us   改变比较值  //CC60:极性相反，低于比较值为高电平，高于比较值为低电平  整个PWM周期600个TICK(6MHz)         
//{
//	//1，3，5步上升。2，4，6步下降。
//	time1++;
//	
//	if(time1 % 104 == 0)   //分为16小步升
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
//	CCU6_T12_Str_En();    //加载比较值
//	
//	if(time1 == 1664) time1 = 0;
//	GPT12E->T3.reg = 64936;//65536-600
//}



//错误理解
//void gpt1_timer3_call_back(void) //33.33us   改变比较值       //极性相反，低于比较值为高电平，高于比较值为低电平  整个PWM周期600个TICK
//{
//	temp3++;
//	
//	time1++;
//	if(time1 == 1) //第二个1/3周期应做出的操作
//	{
//		CCU6->CC60SR.reg = (uint16)0x00; 
//		CCU6->CC61SR.reg = (uint16)0x3c;//0x3c:十进制60 
//		CCU6->CC62SR.reg = (uint16)0x00;
//		CCU6_T12_Str_En(); 
//	}
//	else if(time1 == 2)
//	{
//		CCU6->CC60SR.reg = (uint16)0x00; 
//		CCU6->CC61SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x3c;//0x3c:十进制60 
//		CCU6_T12_Str_En(); 
//	}
//	else if(time1 == 3)
//	{
//		CCU6->CC60SR.reg = (uint16)0x3c;//0x3c:十进制60
//		CCU6->CC61SR.reg = (uint16)0x00;
//		CCU6->CC62SR.reg = (uint16)0x00; 
//		CCU6_T12_Str_En(); 
//		time1 = 0; 	
//	}
//	GPT12E->T3.reg = 65336;
//}


