#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "image.h"
 
 int main(void)
 {	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	POINT_COLOR=RED;
	 while(1)
	 {   //ͼƬ����ʾ�߶ȺͿ��Ϊͼ��ֱ���-1
		 LCD_Color_Fill(0,10,198-1,181-1,(u16*)&gImage_image[0]);//������ǿ��ת��Ϊu16*,ע��ͼ���С���ܳ�����,������ܻ��ܷ�
		 delay_ms(500);
		 LCD_Clear(WHITE);
	   Show_Str(80,120,220,16,"��֮΢Ц",16,0);
		 delay_ms(500);
	 }
}
