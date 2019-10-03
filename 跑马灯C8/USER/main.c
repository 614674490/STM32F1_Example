#include "stm32f10x.h" 
#include "sys.h" 
#include "key.h"
#include "delay.h"
#include "led.h"
 int main(void)
 {
	 LED_Init();
	 delay_init();
	 led1=1;
	 relay=0;
	 while(1)
	 {
		 led1=!led1;
		 delay_ms(500);
		/* relay=!relay;
		 delay_ms(1000);*/
		 /*GPIO_SetBits(GPIOC,GPIO_Pin_5);                   //将PB5置位高电平
		 GPIO_ResetBits(GPIOB,GPIO_Pin_5);                   //将PE5置位高电平
		 delay_ms(500);
		 GPIO_ResetBits(GPIOC,GPIO_Pin_5);                   //将PB5置位高电平
		 GPIO_SetBits(GPIOB,GPIO_Pin_5);                   //将PE5置位高电平
		 delay_ms(500);*/
	 }
}
