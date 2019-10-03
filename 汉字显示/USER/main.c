#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "image.h"
 
 int main(void)
 {	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	POINT_COLOR=RED;
	 while(1)
	 {   //图片的显示高度和宽度为图像分辨率-1
		 LCD_Color_Fill(0,10,198-1,181-1,(u16*)&gImage_image[0]);//将数组强制转化为u16*,注意图像大小不能超区域,否则可能会跑飞
		 delay_ms(500);
		 LCD_Clear(WHITE);
	   Show_Str(80,120,220,16,"迷之微笑",16,0);
		 delay_ms(500);
	 }
}
