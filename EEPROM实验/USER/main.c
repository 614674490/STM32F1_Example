#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h" 
#include "key.h"
#include "lcd.h"
#include "24C02.h"
#include "iic.h"
#include "usart.h"


PID pid;
int main()
{
	u8 key;
	pid.KD.kd=70;
	pid.T.t=200;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	LCD_Init();
	KEY_Init();
	IIC_Init();
	AT24CXX_Init();
//	AT24CXX_Write(0,pid.KD.ch,4);   //初始化时不用屏蔽 第一次烧进程序之后再屏蔽
//	AT24CXX_Write(4,pid.T.ch,4);
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"EEPROM TEST");
	LCD_ShowString(30,70,200,16,16,"KD=");
	LCD_ShowString(30,90,200,16,16,"T=");
	POINT_COLOR=BLUE;
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			pid.KD.kd++;
			AT24CXX_Write(0,pid.KD.ch,4);
		}
		if(key==KEY1_PRES)
		{
			pid.T.t++;
			AT24CXX_Write(4,pid.T.ch,4);
		}
		AT24CXX_Read(0,pid.KD.ch,4);
		AT24CXX_Read(4,pid.T.ch,4);
		LCD_ShowNum(30+3*8,70,pid.KD.kd,5,16);
		LCD_ShowNum(30+2*8,90,pid.T.t,5,16);
	}
}


