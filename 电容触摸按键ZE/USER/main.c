#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tpad.h"

int main(void)
 {	
	LED_Init();
	delay_init();	    	 //��ʱ������ʼ��	
	TAPD_Init(6);        //��ʼ����������
  while(1)
	{
		if(TPAD_Scan(0))      //��������
		{
			led=!led;
		}
	}	 
} 

