#include "timer.h"
#include "led.h"
#include "stm32f10x_tim.h"
/************通用定时器5初始化函数*****************/
void TIM5_Int_Init(u16 arr,u16 psc)   //自动重装载和预分频系数
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);   //使能定时器5
	
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //TIM_CKD_DIV1  设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;            //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);         //使能定时器5的更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM5,ENABLE);                               //使能定时器5
}

/************TIM5 PWM部分初始化函数**************/
void TIM5_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能定时器5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设 不用使能AFIO时钟

	//复用输出功能 输出TIM5_CH1的PWM波形   PA0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;       //复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);   //设置部分重映射 TIM2_CH2->PA1 
	//不用部分重映射来实现，因为硬件管脚有问题，用对应的IO口实现就可以
	 
	//初始化TIM5
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //TIM_CKD_DIV1  设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;            //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	//初始化定时器5通道1的输出比较参数
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;           //PWM模式2
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;    //高电平有效 即比较输出极性为高
	TIM_OC1Init(TIM5,&TIM_OCInitStructure);                     //根据以上参数初始化TIM5 OC1
	/*对于这样的PWM模式2 当CNT>CCR时为高电平 小于则为低电平,CCR越小,占空比就越大（高电平持续时间越长）*/
	
	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);     //使能定时器5在CCR1的预装载定时器
	
	TIM_Cmd(TIM5,ENABLE);                                //使能定时器5
}
	
	



