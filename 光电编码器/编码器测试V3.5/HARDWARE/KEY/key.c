#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//按键输入 驱动代码		   
//技术论坛:www.openedv.com
//修改日期:2011/11/28 
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) ALIENTEK  2009-2019
//All rights reserved
//********************************************************************************
 
void KEY_Init(void) //IO初始化
{
// 	GPIO_InitTypeDef GPIO_InitStructure;
//	//init GPIOA.13,GPIOA.15  上拉输入
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	//init GPIOA.0	  下拉输入
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
//
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	 

}
//u8 KEY_Scan(void)
//{	 
//	static u8 key_up=1;//按键按松开标志	
// 
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
//	if(key_up&&(KEY0==0||KEY1==0||KEY2==1))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		if(KEY0==0)
//		{
//	 
//			 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//			return 1;
//		}
//		else if(KEY1==0)
//		{
//	 	 
//	     	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//			return 2;
//		}
//		else if(KEY2==1)
//		{
//	 
//	    	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//			return 3;
//		}
//	}else if(KEY0==1&&KEY1==1&&KEY2==0)key_up=1; 	    
// 
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//	return 0;// 无按键按下
//}
