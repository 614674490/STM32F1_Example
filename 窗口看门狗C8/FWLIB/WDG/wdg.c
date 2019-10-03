#include "wdg.h"
#include "led.h"
#include "delay.h"
//保存WWDG计数器的设置值,默认为最大. 
u8 WWDG_CNT=0x7f; 
//初始化窗口看门狗 	
//tr   :T[6:0],计数器值 
//wr   :W[6:0],窗口值 
//fprer:分频系数（WDGTB）,仅最低2位有效 
//Fwwdg=PCLK1/(4096*2^fprer). 
void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);           //使能时钟
	
	WWDG_CNT=tr&WWDG_CNT;
	WWDG_SetPrescaler(fprer);                                   	//设置分频系数
	WWDG_SetWindowValue(wr);                                      //设置窗口值
	WWDG_Enable(WWDG_CNT);                                       //使能看门狗，设置初值
	
	//中断初始化分组
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占2，子优先级3，组2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //抢占2，子优先级3，组2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC初始化
	
	WWDG_ClearFlag();                                      //清除中断标志位
	
	WWDG_EnableIT();                                       //使能中断标志位
}

//编写中断处理函数
void WWDG_IRQHander() 
{
	WWDG_SetCounter(WWDG_CNT);                            //进行喂狗
	WWDG_ClearFlag();                                     //清楚中断标志位
	led1=0;
}


