#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "adc.h"
#include "oled.h"
 int main(void)
{
	short temp;
	LED_Init();
	delay_init();
	OLED_GPIO_Init();
  OLED_Init();
	Adc_Init();
	OLED_Display();
	while(1)
	{
		temp=Get_Temprate();          //获取温度值
		if(temp<0) temp=-temp,OLED_P8x16Str(40,3,"-");
		OLED_ShowNum(48,3,temp/100,2,16);
		OLED_ShowNum(72,3,temp%100,2,16);
	}
	
}


