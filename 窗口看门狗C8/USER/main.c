#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "wdg.h"
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();
	LED_Init();
	WWDG_Init(0x7f,0x5f,WWDG_Prescaler_8);         //预分频系数为8
	led=0;
	//led1=1;
	while(1)
	{
    delay_ms(500);
	  led=1;      //此时现象为灯一直亮，因为500ms超过了喂狗时间，因此在500ms之前，系统已经复位，故观察不到灯灭
	}
}


