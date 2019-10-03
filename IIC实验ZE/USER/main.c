#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"                                 //引用延时函数头文件
#include "key.h"
#include "iic.h"
#include "24C02.h"
#include "usart.h"

const u8 TEXT_Buffer[]={"WarShipSTM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)

int main()
{
	u8 key;
	u8 datatemp[SIZE];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	delay_init();
	KEY_Init();
	IIC_Init();
	AT24CXX_Init();
	while(AT24CXX_Check())
	{
		printf("\r\n24C02 Check Failed!\r\n");
		delay_ms(500);
		LED0=!LED0;   //提示24C02未准备就绪
	}
	printf("\r\n24C02 Ready!\r\n");
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)     //写入24C02
		{
			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
			printf("\r\n24C02 Write Finished!\r\n");  //提示写入完成
		}
		if(key==KEY0_PRES)   //读取字符串并显示
		{
			AT24CXX_Read(0,datatemp,SIZE);
			printf("\r\nThe Data Readed Is:%s",datatemp);
		}
		LED1=!LED1;
		delay_ms(200);
	}
	
}


