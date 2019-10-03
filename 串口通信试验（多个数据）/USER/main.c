#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "usart.h"
 int main(void)
{
	u8 rxdata;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //设置NVIC中断分组2:3位抢占优先级，1位响应优先级
	uart_init(115200);                               //设置波特率
	LED_Init();
	delay_init();                                   //延时函数初始化
	led=1;                                         //先关闭LED
	while(1)
	{
		if(USART_RX_BUF[0]==0x55)
		{
			if(USART_RX_BUF[2]==0xaa)
			{
				rxdata=USART_RX_BUF[1];
				if(rxdata==0x01) led=0;
				else if(rxdata==0x00) led=1;
				USART_SendData(USART1,rxdata);
				USART_RX_BUF[0]=0,USART_RX_BUF[2]=0;     //前后两个都要清零
				
			}
		}
	}
}




