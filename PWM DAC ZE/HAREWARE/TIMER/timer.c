#include "timer.h"

//TIM1产生PWM波 高级定时器
void TIM1_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA口和TIM1时钟
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;                            //复用功能输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;                               //PA8->TIM1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;                 //时钟分割
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInitStructure.TIM_Period=arr;                      //重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                 //预分频 不分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	//psc越大 占空比越大
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;          //PWM2模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;    //低电平有效
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;    //比较输出使能
	TIM_OCInitStructure.TIM_Pulse=0;                          //设置待装入捕获比较寄存器的值
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Disable );      //CH1 预装载使能
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);                      //使能ARR上的预装载寄存器
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);                      //MOE 主输出使能 高级定时器必须采用这个
	
	TIM_Cmd(TIM1,ENABLE);                                   //使能定时器1
}
//设置输出电压
//vol:0~330,代表0~3.3V
void PWM_DAC_Set(u16 vol)
{
	float temp=vol;
	temp/=100;
	temp=temp*256/3.3;
	TIM_SetCompare1(TIM1,temp);
}

