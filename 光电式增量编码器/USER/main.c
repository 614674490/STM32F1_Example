#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "key.h"
#include "timer.h"
#include "usart.h"
#include "led.h"

u32	times=0;
u32 temp=0;
void TIM3_IRQHandler(void)                     //定时器3中断处理函数
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)   //检查是否产生更新中断 即计数器是否溢出
	{
		times++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);     //清除中断标志位
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	LED_Init();
	TIM3_Encoder_Init(65535,0);   //65535溢出 不分频，每一个跳变记录一次
	uart_init(115200);
	relay=0;
	while(1)
	{
		relay=!relay;
		delay_ms(500);
		temp=times*65535;
		temp=temp+TIM_GetCounter(TIM3);
		printf("HIGH:%d \r\n",temp);    //输出高电平时长
	}
}









