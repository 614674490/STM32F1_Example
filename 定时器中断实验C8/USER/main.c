#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "timer.h"
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
	led=1,relay=0;   //�ȹرռ̵����͵�
	TIM2_Int_Init(4999,7199);     //500ms   ���Ƶ�
	TIM3_Int_Init(9999,7199);     //1s       ���Ƽ̵���
	while(1);
}


