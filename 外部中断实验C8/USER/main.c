#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exti.h"
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	LED_Init();
	KEY_Init();
	EXTIX_Init();
	led=0;   //先点亮LED灯
	while(1)
	{
		delay_ms(10);
	}
	
}


