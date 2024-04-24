#ifndef MOTOR_H
#define MOTOR_H

#include "tle_device.h"

//占空比
#define PWM_DC (0.1)
//CCU6的是24M，100us有2400个TICK
#define PWM_HIGH_TICK (2400*PWM_DC)
//当电频率为1Hz时，一步的微秒数  166666   //20000
#define STEPTIME 166666



void Motor_Init(void);
#endif

