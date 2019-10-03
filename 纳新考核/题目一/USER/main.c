#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "key.h"
int main(void)
{
	u8 k;
	delay_init();
	LED_Init();
	KEY_Init();
	led=0;  //œ»πÿ±’
	while(1)
	{
		k=KEY_Scan(0);
		if(k==KEY0_PRES)
		{
			led=!led;
		}
	}	
}






