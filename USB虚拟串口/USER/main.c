#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //引用延时函数头文件
#include "usart.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"	 
int main()
{
	u16 t;
	u16 len;	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	USB_Port_Set(0); 	//USB先断开
	delay_ms(700);
	USB_Port_Set(1);	//USB再次连接
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();
  while(bDeviceState != CONFIGURED) printf("USB discontacted\r\n");
	printf("USB contacted\r\n");                                           //检测USB是否连接
	while(1)
	{
		LED0=0;
		if(USB_USART_RX_STA&0x8000)
		{					  
		  LED0=1;
			len=USB_USART_RX_STA&0x3FFF;//得到此次接收到的数据长度
			usb_printf("\r\n您发送的消息为:%d\r\n\r\n",len);
			for(t=0;t<len;t++)
			{
				USB_USART_SendData(USB_USART_RX_BUF[t]);//以字节方式,发送给USB 
			}
			usb_printf("\r\n\r\n");//插入换行
			USB_USART_RX_STA=0;
		}
	}
}


