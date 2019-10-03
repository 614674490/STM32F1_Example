#include "beep.h"
#include "delay.h"
#include "stm32f10x.h"
#include "sys.h"
int main(void)
{
	delay_init();
	BEEP_Init();
	while(1)
	{
		/*******************Î»²Ù×÷***************/
		/*beep=0;
		delay_ms(100);
		beep=1;
		delay_ms(100);*/
		/***************¿âº¯Êý*********************/
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);                   
		delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin_8);                   
		delay_ms(100);
	}
}


 
