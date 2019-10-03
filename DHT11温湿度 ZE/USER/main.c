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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	LCD_Init();
	POINT_COLOR=RED;		//设置字体为红色 
	while(DHT11_Init())
	{
		LCD_ShowString(30,50,200,16,16,"DHT11_Init Error");
		delay_ms(200);
	}
	LCD_ShowString(30,50,200,16,16,"DHT11_Init OK   ");
	POINT_COLOR=BLUE;//设置字体为蓝色 
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
		
	


