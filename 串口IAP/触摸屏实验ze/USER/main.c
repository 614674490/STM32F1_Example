#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "w25qxx.h"
#include "touch.h"
#include "display.h"

 int main(void)
 {	 	
  SCB->VTOR=SRAM_BASE|0x1000;	 	 
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	 
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();	
	KEY_Init();	 	
 	tp_dev.init();

	if(tp_dev.touchtype&0X80)ctp_test();	//����������
	else rtp_test(); 						//����������
}


