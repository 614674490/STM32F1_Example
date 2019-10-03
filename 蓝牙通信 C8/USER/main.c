#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart2.h"
#include "usart.h"
#include "string.h"	
int main(void)
{
	u8 k;
	u8 i;
	delay_init();
	LED_Init();
	KEY_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	USART2_Init(9600);
	led=0;  //�ȹر�
	while(1)
	{
		k=KEY_Scan(0);
		if(k==KEY0_PRES)
		{
			u2_printf("HC-05 Test:%s\n",USART2_RX_BUF);
		}
		if(USART2_RX_STA&0x8000)
		{
			if(rx_buf[0]==0x01) led=0;
			else if(rx_buf[0]==0x00) led=1;
			for(i=0;i<len-2;i++) rx_buf[i]=0;
			USART2_RX_STA=0;
			flag=0;
		}
			
	}	
}






