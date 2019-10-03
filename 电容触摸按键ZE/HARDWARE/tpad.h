#ifndef _tpad_h
#define _tpad_h
#include "sys.h"

void TIM5_Cap_Init(u16 arr,u16 psc);

u8 TAPD_Init(u16 psc);  //初始化触摸按键
void TPAD_Reset(void);    //复位函数
u16 TPAD_Get_Val(void);    //获得充电时间
u16 TPAD_Get_MaxVal(u8 n);   //读取n次获得充电最大时长
u8 TPAD_Scan(u8 mode);     //扫描触摸按键

#endif


