#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "Encoder.h"

//ALIENTEK 探索者STM32F407开发板 实验0
//STM32F4工程模板-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main(void)
{ 
	u32 oldcnt2;
	uart_init(115200);
	delay_init(84);
	Encoder_Init();
  while(1)
		{
				delay_ms(100);
       if(oldcnt2!=TIM3->CNT) 
		  { 
			oldcnt2=TIM3->CNT; 
			printf("encoder2:%d\r\n",oldcnt2); 
			printf("\r\n");
	    }
    }
	}











	