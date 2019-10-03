#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "key.h"
#include "timer.h"
#include "usart.h"
#include "led.h"

u32	times=0;
u32 temp=0;
void TIM3_IRQHandler(void)                     //��ʱ��3�жϴ�����
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)   //����Ƿ���������ж� ���������Ƿ����
	{
		times++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);     //����жϱ�־λ
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
	TIM3_Encoder_Init(65535,0);   //65535��� ����Ƶ��ÿһ�������¼һ��
	uart_init(115200);
	relay=0;
	while(1)
	{
		relay=!relay;
		delay_ms(500);
		temp=times*65535;
		temp=temp+TIM_GetCounter(TIM3);
		printf("HIGH:%d \r\n",temp);    //����ߵ�ƽʱ��
	}
}









