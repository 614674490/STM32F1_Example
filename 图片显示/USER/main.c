#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "gif.h"
 
 int main(void)
 {	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	 while(1)
	 {
	   LCD_Color_Fill(0,0,160-1,120-1,(u16*)&gImage_gif[0]);//������ǿ��ת��Ϊu16*,ע��ͼ���С���ܳ�����,�����
		 delay_ms(200);
	 }
}
