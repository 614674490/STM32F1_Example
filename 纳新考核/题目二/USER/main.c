#include "stm32f10x.h" 
#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exti.h"
int main(void)
{
	delay_init();
	LED_Init();
	KEY_Init();
	EXTIX_Init();
	led=0;  //œ»πÿ±’
	while(1)
	{
		;
	}	
}






