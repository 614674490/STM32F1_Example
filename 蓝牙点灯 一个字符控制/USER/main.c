#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usart3.h"


 int main(void)
 {	
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//usart3接收中断优先级2 次优先级2
	 usart2_init(9600);//蓝牙的串口初始化 注意手机与单片机的蓝牙连接波特率是要9600
	while(1)
	{
	   if(USART2_RX_STA&0x8000)
		 {
		   if(USART2_RX_BUF[0]=='0')
			 {
			  LED1=!LED1;
			 }
			 USART2_RX_STA=0;
		 }
	}
 }

