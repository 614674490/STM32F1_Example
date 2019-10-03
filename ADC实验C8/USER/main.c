#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "adc.h"
#include "oled.h"
u16 adcx;
float temp;
 int main(void)
{
	
	
	LED_Init();
	delay_init();
	OLED_GPIO_Init();
  OLED_Init();
	Adc_Init();
	OLED_Display();
	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_1,10);
		OLED_ShowNum(32,2,adcx,4,16);             //显示直接获取的数值
		temp=(float)(adcx*(3.3/4096));            //转换成电压
		adcx=temp;                               //获取整数部分
		OLED_ShowNum(32,6,adcx,1,16);
		temp-=adcx;                         //获取小数部分
		temp=temp*1000;                          //精确到千分位
		OLED_ShowNum(48,6,temp,3,16);             //显示小数
	}
	
}


