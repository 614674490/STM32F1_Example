#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "dht11.h" 	
int main()
{
	u8 temperature;  	    
	u8 humidity;   
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();
	LCD_Init();
	POINT_COLOR=RED;		//��������Ϊ��ɫ 
	while(DHT11_Init())
	{
		LCD_ShowString(30,50,200,16,16,"DHT11_Init Error");
		delay_ms(200);
	}
	LCD_ShowString(30,50,200,16,16,"DHT11_Init OK   ");
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_ShowString(30,90,200,16,16,"Temp:  C");
	LCD_ShowString(30,110,200,16,16,"Humi:  %");
	while(1)
	{
		delay_ms(100);
		DHT11_Read_Data(&temperature,&humidity);
		LCD_ShowNum(30+40,90,temperature,2,16);
		LCD_ShowNum(30+40,110,humidity,2,16);
		LED0=!LED0;
	}
}
		
	


