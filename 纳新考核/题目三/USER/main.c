#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "usart1.h"
int main(void)
{
	u16 i;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //设置为第2组  2位抢占 2位响应
	My_USART1_Init();
	LED_Init();
	led=1;
	while(1)
	{
//		if(USART1_RX_STA&0x8000)
//		 {
//			 led=0;
//			 len=USART1_RX_STA&0x7fff;
//			 if((USART1_RX_BUF[0]==0xaa)&&(USART1_RX_BUF[len-1]==0x55))
//			 {
//				 led=0;
//				 for(i=0;i<len;i++)
//			   {
//				   USART_SendData(USART1,USART1_RX_BUF[i]);		
//				   while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束		该句不能省！！		 
//			   }
//			 }
//			 USART1_RX_STA=0;
//		 }
		if(USART1_RX_STA&0x8000)
		 {
			 led=!led;
			 for(i=0;i<len;i++)
			 {
				 USART_SendData(USART1,USART1_RX_BUF[i]);		
				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束		该句不能省！！		 
			 }
			 USART1_RX_STA=0;
			 flag=0;
		 }
	 }				 
}


