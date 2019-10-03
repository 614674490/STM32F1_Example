#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "rtc.h"
/*�������ⲿ��Դ*/
 int main(void)
{
	u8 t=0;
	SCB->VTOR=FLASH_BASE|0x10000;     //�����ж�������ƫ�Ƶ�ַ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
  OLED_GPIO_Init();
  OLED_Init();
	OLED_Display();
	RTC_Init();                                  //ʱ�ӳ�ʼ��  ��ѭ��
	LED0=0;
	while(1)
	{
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			OLED_ShowNum(0,2,calendar.w_year,4,16);									  
			OLED_ShowNum(40,2,calendar.w_month,2,16);									  
			OLED_ShowNum(64,2,calendar.w_date,2,16);	 
			switch(calendar.week)
			{
				case 0:
					OLED_P8x16Str(40,4,"Sunday   ");
					break;
				case 1:
					OLED_P8x16Str(40,4,"Monday   ");
					break;
				case 2:
					OLED_P8x16Str(40,4,"Tuesday  ");
					break;
				case 3:
					OLED_P8x16Str(40,4,"Wednesday");
					break;
				case 4:
					OLED_P8x16Str(40,4,"Thursday ");
					break;
				case 5:
					OLED_P8x16Str(40,4,"Friday   ");
					break;
				case 6:
					OLED_P8x16Str(40,4,"Saturday ");
					break;  
			}
			OLED_ShowNum(40,6,calendar.hour,2,16);									  
			OLED_ShowNum(64,6,calendar.min,2,16);									  
			OLED_ShowNum(88,6,calendar.sec,2,16);
		}	
	}
}


