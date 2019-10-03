#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "usart.h"
 int main(void)
{
	u8 rxdata;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //����NVIC�жϷ���2:3λ��ռ���ȼ���1λ��Ӧ���ȼ�
	uart_init(115200);                               //���ò�����
	LED_Init();
	delay_init();                                   //��ʱ������ʼ��
	led=1;                                         //�ȹر�LED
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
				USART_RX_BUF[0]=0,USART_RX_BUF[2]=0;     //ǰ��������Ҫ����
				
			}
		}
	}
}




