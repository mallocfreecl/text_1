#ifndef MOTOR_H
#define MOTOR_H

#include "tle_device.h"

//ռ�ձ�
#define PWM_DC (0.1)
//CCU6����24M��100us��2400��TICK
#define PWM_HIGH_TICK (2400*PWM_DC)
//����Ƶ��Ϊ1Hzʱ��һ����΢����  166666   //20000
#define STEPTIME 166666



void Motor_Init(void);
#endif

