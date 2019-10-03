#include "wkup.h"
#include "led.h"
#include "delay.h"
//待机模式
void Sys_Standby(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);   //使能电源时钟
	PWR_WakeUpPinCmd(ENABLE);                           //设置WK_UP引脚作为唤醒源
	PWR_EnterSTANDBYMode();                            //进入待机模式
}

//系统进入待机模式
void Sys_Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口
	Sys_Standby();
}

//检测WKUP脚的信号
//返回值1:连续按下3s以上
//      0:错误的触发	
u8 Check_WKUP(void) 
{
	u8 t=0;
	while(1)
	{
		if(WKUP_KD)
		{
			t++;
			delay_ms(30);
			if(t>=100)                 //按键按下超过三秒钟
			{
				return 1;
			}
		}
			else
			{
				return 0;                    //否则返回1
			}
		}
}

//PA0 WKUP唤醒初始化
void WKUP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);   //使能GPIOA和AFIO
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;    //下拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//外部中断0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	//优先级分组
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	if(Check_WKUP()==0)  Sys_Enter_Standby();   //刚开机时，未按按键，直接进入待机模式
}

//中断处理函数 未进入待机模式前 WK_UP为普通IO口 需通过中断来进入待机模式 而进入之后 该键为唤醒键
void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);                  //清除中断标志位
	if(Check_WKUP())    //为1则表示进入待机状态
		Sys_Enter_Standby();
}

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


