#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "adc.h"
#include "oled.h"
#include "usart.h"
 int main(void)
{
	u16 adcx,adcx2;
	float temp,temp2;
	LED_Init();
	delay_init();
	IIC_Init();
  OLED_Init();
	Adc_Init();
	uart_init(115200); 
	OLED_Display();
	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_1,10);
		OLED_ShowNum(32,0,adcx,4,16);             //��ʾֱ�ӻ�ȡ����ֵ
		temp=(float)(adcx*(3.3/4096));            //ת���ɵ�ѹ
		adcx=temp;                               //��ȡ��������
		OLED_ShowNum(32,2,adcx,1,16);
		temp-=adcx;                         //��ȡС������
		temp=temp*1000;                          //��ȷ��ǧ��λ
		OLED_ShowNum(48,2,temp,3,16);             //��ʾС��
	}
	
}


