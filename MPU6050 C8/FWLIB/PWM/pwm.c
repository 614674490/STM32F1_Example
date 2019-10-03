#include "pwm.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
u32 dissz[8];//超声波返回数组
int dis;
u8 delayout;//中断函数进入标志

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure;  
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /* 定时器2配置 */                                            
    TIM_TimeBaseStructure.TIM_Period =arr; //初值 计数溢出值 //50hz
    TIM_TimeBaseStructure.TIM_Prescaler = psc;   //分频值       //50hz                          
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                              
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数模式为向上计数               
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    /* PWM1 配置 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//pwm1模式                       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = 0;                                  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High; 

	
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //A6
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIM3在CCR1上的预装载寄存器
    TIM_OC2Init(TIM3, &TIM_OCInitStructure); //A7
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIM3在CCR2上的预装载寄存器 
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //B0
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIM3在CCR3上的预装载寄存器 
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //B1
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIM3在CCR4上的预装载寄存器 
			/* TIM3 enable counter */  
		TIM_Cmd(TIM3, ENABLE);                                           
}//舵机控制end

//***************************************************************************************************
//void tim4(void)//定时器4初始化函数生成精确延时
//{
//	
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
//    /* Time base configuration */                                            
//		TIM_TimeBaseStructure.TIM_Period =0;  
//    TIM_TimeBaseStructure.TIM_Prescaler = 35999;                                    
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                              
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;               
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//	  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
// 
//定时器1初始化函数end
//}

void jqdelay(int delaytime)//定时器生成精确延时
{
  delayout=0;
	TIM4->ARR=delaytime*2;//寄存器为定时器初值
  TIM_Cmd(TIM4,ENABLE);
	while(delayout==0);//判断是否进入中断
	TIM_Cmd(TIM4,DISABLE);
	
}
int csbcj_w(void)//前超声波测距
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	delay_ms(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	
 	delay_ms(10);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//t口发送脉冲

		
  TIM1->CNT =0;//清空tim3计数器寄存器CNT为计数寄存器
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0);//等待e口数据到达
	TIM_Cmd(TIM1,ENABLE);//使能tim3开始计数
  while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1)&&(TIM1->CNT<TIM1->ARR-10));//ARR为定时器初值寄存器
  TIM_Cmd(TIM1,DISABLE);//关闭tim3
	dis=TIM1->CNT/58;
	//printf("sound return distance=%d\r\n",dis);
  return dis;
}

void tim1(void)//定时器1初始化函数 存储超声波距离
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
    /* Time base configuration */                                            
		TIM_TimeBaseStructure.TIM_Period =58000;  //最大距离为该值除以58
    TIM_TimeBaseStructure.TIM_Prescaler =72;                                    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                              
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;               
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
		TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
}
void timconfig(void)//定时器初始化汇总
{
tim1();


}//定时器初始化汇总end


