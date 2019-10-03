#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "usart1.h"
#include "dma.h"
#include "stm32f10x_dma.h"
 int main(void)
{
	u8 i,j=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	LED_Init();
	led=1;
	My_USART1_Init();                                     //此处每次传输的数据必须是8位，不达到8位不算是传输成功
	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)USART1_RX_BUF,8);//DMA1通道5,外设为串口1,存储器为USART1_RX_BUF,长度8
	while(1)
	{
		MYDMA_Enable(DMA1_Channel5);
		while(1)
		{
			if(DMA_GetFlagStatus(DMA1_FLAG_TC5)!=RESET)	//判断通道5传输完成
			{
				DMA_ClearFlag(DMA1_FLAG_TC5);//清除通道5传输完成标志
				break; 
			}
		}
		while(USART1_RX_BUF[j]!=0x55)	j++;
		if((USART1_RX_BUF[0]==0xaa)&&(USART1_RX_BUF[j]==0x55))
		 {
       TIM3_Int_Init(1999,7199);     //200ms
			 for(i=0;i<=j;i++)
			 {
				 USART_SendData(USART1,USART1_RX_BUF[i]);
				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束		该句不能省！！		
			 }
			 j=0;
		 }
	 }
}
//DMA只能传输规定长度的信息


