#include "stm32f10x.h"
#include "led.h"
#include "delay.h"     //������ʱ����ͷ�ļ�
#include "ds18b20.h"
#include "oled.h"
int main()
{
	short temperature;
	delay_init();       //Ҫ��OLED��ʼ������ǰ�� ����������ѭ��
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
		delay_ms(100);          //ÿ100ms���һ��
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
		
		


