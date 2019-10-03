#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
void jqdelay(int delaytime);
int csbcj_w(void);
void TIM3_PWM_Init(u16 arr,u16 psc);
void timconfig(void);
#endif
