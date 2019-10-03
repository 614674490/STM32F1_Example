#include "timer.h"
#include "led.h"
#include "stm32f10x.h"
#include "usart.h"
#include "stm32f10x_tim.h"
void TIM3_Encoder_Init(u16 arr,u16 psc)    //编码器模式初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	//初始化IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;              //上拉输入  此时PA6 7为高电平 等待变为低电平       
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;         
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化定时器3
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //设置时钟分割 TDTS=0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //设置计数模式 向上计数
	TIM_TimeBaseStructure.TIM_Period=arr;                      //自动重载值  改动
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                   //预分频
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//编码器模式3 下降沿触发
	TIM_ICStructInit(&TIM_ICInitStructure);   //根据默认设置参数
	TIM_ICInitStructure.TIM_ICFilter=0x00;                     //配置输入滤波器 不滤波
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//中断初始化分组
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;      //优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);      //清除标志位         //清除中断标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);      //允许更新中断    //使能中断
	
	TIM_Cmd(TIM3,ENABLE);                                   //使能定时器3
}




	
				
	
	