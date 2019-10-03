#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exti.h"
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
	KEY_Init();
	EXTIX_Init();
	led=0;   //�ȵ���LED��
	while(1)
	{
		delay_ms(10);
	}
	
}


