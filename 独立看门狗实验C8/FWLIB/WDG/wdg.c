#include "wdg.h"

void WDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);             //写入0x5555 取消写保护
	
	IWDG_SetPrescaler(prer);                                  //设置预分频系数 确定时钟
	
	IWDG_SetReload(rlr);                                      //设置重装载值   确定溢出时间
	
	IWDG_ReloadCounter();                                     //应用程序喂狗
	
	IWDG_Enable();                                            //使能看门狗
}




	
