#include "timer.h"
#include "led.h"
#include "stm32f10x_tim.h"
void TIM2_Int_Init(u16 arr,u16 psc)   //自动重装载和预分频系数
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //使能定时器2
	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;            //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);         //使能定时器2的更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);                               //使能定时器2
}
void TIM3_Int_Init(u16 arr,u16 psc)   //自动重装载和预分频系数
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //使能定时器3
	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;            //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);         //使能定时器3的更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);                               //使能定时器3
}
/********编写定时器2中断服务函数*************************/	
void TIM2_IRQHandler(void)
{
  //u8 i=0;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)     //检查TIM2更新中断是否发生
	{
		//i++;
		//if(i==10)
		//{
			led=!led;
		//	i=0;
		//}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);      //清除TIM2更新标志位
	}
}
/*******************编写定时器3中断服务函数**************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)     //检查TIM3更新中断是否发生
	{
		relay=!relay;
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);      //清除TIM3更新标志位
	}
}




