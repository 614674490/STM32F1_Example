#include "sys.h"
#include "led.h"
#include "delay.h"   
#include "usart.h"
#include "lcd.h"
#include "stmflash.h"
#include "key.h"
//要写入到STM32 FLASH的字符串数组
const u8 TEXT_Buffer[]={"STM32F103 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)		//数组长度
#define FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
int main()
{
	u8 key;
	u8 datatemp[SIZE];
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	LCD_Init();
	KEY_Init();
	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"KEY0:Write  KEY1:Read");        //y; 0-303 x;0-200
	POINT_COLOR=GREEN;			//设置字体为黄色
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)              //写入数据
		{
			LCD_Fill(30,90,239,110+16,WHITE);//清除数据     
			LCD_ShowString(30,90,200,16,16,"Start Write FLASH.... ");
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);
			LCD_ShowString(30,90,200,16,16,"FLASH Write Fainsh....");
		}
		if(key==KEY1_PRES)            //读取字符串并显示
		{
			LCD_ShowString(30,90,200,16,16,"Start Read FLASH....  ");
			STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
			LCD_ShowString(30,90,200,16,16,"The Data is:          ");
			LCD_ShowString(30,110,200,16,16,datatemp);
		}
		delay_ms(200);
		LED0=!LED0;
	}
	

}


