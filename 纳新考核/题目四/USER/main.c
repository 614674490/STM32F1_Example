#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //引用延时函数头文件
#include "time.h"
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	LED_Init();
	TIM5_PWM_Init(199,7199);     //20ms
	while(1)
	{
		TIM_SetCompare1(TIM5,183);     //8.04%
	}

}


