#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usart3.h"


 int main(void)
 {	
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//usart3�����ж����ȼ�2 �����ȼ�2
	 usart2_init(9600);//�����Ĵ��ڳ�ʼ�� ע���ֻ��뵥Ƭ�����������Ӳ�������Ҫ9600
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

