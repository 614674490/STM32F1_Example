#include "sys.h"
#include "led.h"
#include "delay.h"   
#include "usart.h"
#include "lcd.h"
#include "stmflash.h"
#include "key.h"
//Ҫд�뵽STM32 FLASH���ַ�������
const u8 TEXT_Buffer[]={"STM32F103 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)		//���鳤��
#define FLASH_SAVE_ADDR  0X08070000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
int main()
{
	u8 key;
	u8 datatemp[SIZE];
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();
	LCD_Init();
	KEY_Init();
	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"KEY0:Write  KEY1:Read");        //y; 0-303 x;0-200
	POINT_COLOR=GREEN;			//��������Ϊ��ɫ
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)              //д������
		{
			LCD_Fill(30,90,239,110+16,WHITE);//�������     
			LCD_ShowString(30,90,200,16,16,"Start Write FLASH.... ");
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);
			LCD_ShowString(30,90,200,16,16,"FLASH Write Fainsh....");
		}
		if(key==KEY1_PRES)            //��ȡ�ַ�������ʾ
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


