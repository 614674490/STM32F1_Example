#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "beep.h"
 int main(void)
{
	delay_init();
	BEEP_Init();
	while(1)
	{
		/*******************位操作***************/
		/*beep=0;
		delay_ms(100);
		beep=1;
		delay_ms(100);*/
		/***************库函数*********************/
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);                   //将PE5置位高电平
		delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin_5);                   //将PE5置位高电平
		delay_ms(100);
	}
	
}
