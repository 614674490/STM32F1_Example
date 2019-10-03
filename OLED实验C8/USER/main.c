#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "oled.h"
 int main(void)
{
	delay_init();
	LED_Init();
	OLED_GPIO_Init();
  OLED_Init();
	OLED_ShowNum(10,0,123,3,16);
	while(1)
	{
		led=!led;
	}
}


