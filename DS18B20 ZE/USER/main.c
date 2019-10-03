#include "stm32f10x.h"
#include "led.h"
#include "delay.h"     //引用延时函数头文件
#include "ds18b20.h"
#include "oled.h"
int main()
{
	short temperature;
	delay_init();       //要在OLED初始化函数前面 否则会进入死循环
	LED_Init();
	OLED_GPIO_Init();
	OLED_Init();
	OLED_Display();
	while(DS18B20_Init())
	{
		OLED_P8x16Str(0,0,"DS18B20 Error");
	}
	OLED_P8x16Str(0,0,"DS18B20 OK   ");
	while(1)
	{
		delay_ms(100);          //每100ms检测一次
		temperature=DS18B20_Get_Temp();
		if(temperature<0)
		{
			OLED_P8x16Str(40,4,"-");
			temperature=-temperature;
		}
		else OLED_P8x16Str(40,4,"+");
		OLED_ShowNum(48,4,temperature/10,2,16);
		OLED_ShowNum(72,4,temperature%10,1,16);
		LED0=!LED0;
	}
}
		
		


