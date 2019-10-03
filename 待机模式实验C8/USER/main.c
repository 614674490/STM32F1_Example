#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "wkup.h"
u8 i=0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)     //检查TIM2更新中断是否发生
	{
		i++;
		if(i==30)
		{
			Sys_Standby();                         //30s之后进入待机模式
			i=0;
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);      //清除TIM2更新标志位
	}
}
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_Init();
	delay_init();
	TIM2_Int_Init(9999,7199);     //1s
	//WKUP_Init();
	while(1)
	{
		led=0;
	}
	
}


