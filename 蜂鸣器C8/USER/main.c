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
		/*******************λ����***************/
		/*beep=0;
		delay_ms(100);
		beep=1;
		delay_ms(100);*/
		/***************�⺯��*********************/
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);                   //��PE5��λ�ߵ�ƽ
		delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin_5);                   //��PE5��λ�ߵ�ƽ
		delay_ms(100);
	}
	
}
