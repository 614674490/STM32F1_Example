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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
	led=1;
	My_USART1_Init();                                     //�˴�ÿ�δ�������ݱ�����8λ�����ﵽ8λ�����Ǵ���ɹ�
	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)USART1_RX_BUF,8);//DMA1ͨ��5,����Ϊ����1,�洢��ΪUSART1_RX_BUF,����8
	while(1)
	{
		MYDMA_Enable(DMA1_Channel5);
		while(1)
		{
			if(DMA_GetFlagStatus(DMA1_FLAG_TC5)!=RESET)	//�ж�ͨ��5�������
			{
				DMA_ClearFlag(DMA1_FLAG_TC5);//���ͨ��5������ɱ�־
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
				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���		�þ䲻��ʡ����		
			 }
			 j=0;
		 }
	 }
}
//DMAֻ�ܴ���涨���ȵ���Ϣ


