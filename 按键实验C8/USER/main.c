#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "key.h"
int main(void)
{
	vu8 key;    //uchar
	delay_init();
	LED_Init();
	KEY_Init();
	relay=1;  //�ȹر�
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
			relay=!relay;
	/*	delay_ms(100);
		if(KEY0==0)
		{
			delay_ms(50);
			if(KEY0==0)
			  relay=!relay;
		}*/
		/*key=KEY_Scan(0);     //��֧��������
		if(key)
		{
			switch(key)
			{
				case KEY0_PRES:      //����LED0����
					relay=!relay;
  				break;
			  case WK_UP_PRES:     //����LED1����
					relay=1;
					break;
			} 
		}*/
		/*if(KEY0==0) relay=1;
		else relay=0;*/
		/*if(WK_UP==1)  relay=0;
		else relay=1;*/
	}	
}






