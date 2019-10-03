#include "Encoder.h"
///////////////////////////////////////////////////////
//日期:2015.10.11
//地点：汕头大学
//作者：姚利

//////////////////////////////////////////////////////
void Encoder_Init(void)
{
  GPIO_InitTypeDef         GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启TIM2时钟
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//开启GPIOA的时钟 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);//开启GPIOA0的TIM2时钟引脚
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);//开启GPIOA1的TIM2时钟引脚
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //GPIOA0,GPIOA1
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//配置端口输出类型 0 输出推挽（复位状态）  1输出开漏
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;//配置端口上拉下拉的寄存器，00无上拉下拉，01上拉，10 下拉 11保留
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA0  PA1

	
  TIM_TimeBaseStructure.TIM_Period = 60000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	//设置定时器2为编码器模式  IT1 IT2为上升沿计数
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;  //输入滤波器
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);  //清除所有标志位
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许中断更新
  TIM3->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}






























