#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "Encoder.h"

//ALIENTEK ̽����STM32F407������ ʵ��0
//STM32F4����ģ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

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











	