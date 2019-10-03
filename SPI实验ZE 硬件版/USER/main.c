#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "spi.h"
#include "w25qxx.h"
#include "usart.h"

//要写入到W25Q64的字符串数组
const u8 TEXT_Buffer[]={"WarShipSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)
	
int main(void)
{
	u8 key=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	KEY_Init();
	SPI2_Init();
	delay_init();
	W25QXX_Init();
	
	printf("\r\nFlash类型 : ");            //打印芯片类型
	switch(W25QXX_ReadID()){
		case W25Q80:
			printf("W25Q80");
		break;
		case W25Q16:
			printf("W25Q16");
		break;
		case W25Q32:
			printf("W25Q32");
		break;
		case W25Q64:
			printf("W25Q64");
		break;
		case W25Q128:
			printf("W25Q128");
		break;
	}
	printf("\r\n");
  FLASH_SIZE=128*1024*1024;	//FLASH 大小为16M字节
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);			//从倒数第100个地址处开始,写入SIZE长度的数据
			printf("\r\nW25Q128 Write Finished!\r\n");
		}
		if(key==KEY0_PRES)
		{
			W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//从倒数第100个地址处开始,读出SIZE个字节
			printf("\r\nThe Data Readed Is:");
			printf("%s\r\n",datatemp);
		}
		delay_ms(200);
		LED0=!LED0;
	}
}

