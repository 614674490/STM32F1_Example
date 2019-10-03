#ifndef _wkup_h
#define _wkup_h
#include "sys.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#define WKUP_KD PAin(0)         //用于检测外部按键是否按下

void EXTI0_IRQHandler(void);   //中断处理函数
void WKUP_Init(void);        //待机模式初始化
u8 Check_WKUP(void);      //检测唤醒键信号
void Sys_Enter_Standby(void);   //系统进入待机模式
void Sys_Standby(void);        //待机模式

void TIM2_Int_Init(u16 arr,u16 psc);



#endif



