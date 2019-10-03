#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "timer.h"
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	LED_Init();
	led=1,relay=0;   //先关闭继电器和灯
	TIM2_Int_Init(4999,7199);     //500ms   控制灯
	TIM3_Int_Init(9999,7199);     //1s       控制继电器
	while(1);
}


