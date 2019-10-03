#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tpad.h"

int main(void)
 {	
	LED_Init();
	delay_init();	    	 //延时函数初始化	
	TAPD_Init(6);        //初始化触摸按键
  while(1)
	{
		if(TPAD_Scan(0))      //不连续按
		{
			led=!led;
		}
	}	 
} 

