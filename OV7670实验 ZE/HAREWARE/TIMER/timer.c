#include "timer.h"
#include "usart.h"

void TIM2_Int_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);   //使能更新中断和触发中断
	
	TIM_Cmd(TIM2,ENABLE);                                  //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;    //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

u8 ov_frame; 	//统计帧数
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		printf("frame: %dfps\r\n",ov_frame);
		ov_frame=0;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}






