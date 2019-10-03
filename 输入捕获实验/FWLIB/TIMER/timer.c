#include "timer.h"
#include "led.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "usart.h"
void TIM2_Cap_Init(u16 arr,u16 psc)
{
	//TIM2_CH2->PA1
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	//初始化IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;         //PA1 下拉输入  不用设置速度
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;             //PA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						       //PA1 上拉
	
	//初始化定时器2
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //设置时钟分割 TDTS=0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //设置计数模式 向上计数
	TIM_TimeBaseStructure.TIM_Period=arr;                      //自动重载值
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                   //预分频
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//初始化输入捕获通道
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;             //CC1S=01 选择输入端 IC2映射到TI2上 通道2
	TIM_ICInitStructure.TIM_ICFilter=0x00;                     //配置输入滤波器 不滤波
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //设置捕获方式 上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;        //配置输入分频 不分频
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI ;  //选择映射 映射到TI2上 通道2要映射到通道1选择间接，反之直接
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	//中断初始化分组
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;      //优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);      //允许更新中断和CC2E捕获中断
	
	TIM_Cmd(TIM2,ENABLE);                                   //使能定时器2
}

u32  TIM2CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH2_CAPTURE_VAL;	//输入捕获值

void TIM2_IRQHandler(void)                     //定时器2中断处理函数
{
	if((TIM2CH2_CAPTURE_STA&0x80)==0)             //还未成功捕获
	{
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)   //产生更新中断 超过65535 溢出中断
		{
			if(TIM2CH2_CAPTURE_STA&0x40)                  //如果已经捕获到高电平
			{
				if((TIM2CH2_CAPTURE_STA&0x3F)==0x3F)          //如果高电平时间太长
				{
					TIM2CH2_CAPTURE_STA|=0x80;                  //标记成功捕获高电平一次
					TIM2CH2_CAPTURE_VAL=0xFFFF;
				}
				else TIM2CH2_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)   //产生捕获中断  上次捕获到高电平
		{
			if(TIM2CH2_CAPTURE_STA&0x40)                  //如果捕获到一个下降沿
			{
				TIM2CH2_CAPTURE_STA|=0x80;                 //标记成功捕获到一次高脉宽
				TIM2CH2_CAPTURE_VAL=TIM_GetCapture1(TIM2);
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);  //改为上升沿捕获
			}
			else                                            //上一次未捕获到高电平 第一次开始捕获高电平
			{
				TIM2CH2_CAPTURE_STA=0;	//清空		    				
      	TIM2CH2_CAPTURE_VAL=0;	
			  TIM_SetCounter(TIM2,0);         //将目前定时器的值设置为0
				TIM2CH2_CAPTURE_STA|=0x40;      //标记捕获到了上升沿
	      TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);  //改为下降沿捕获
			}
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC2);     //清除中断标志位
}
	
				
	
	