#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //������ʱ����ͷ�ļ�
#include "time.h"
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
	TIM5_PWM_Init(199,7199);     //20ms
	while(1)
	{
		TIM_SetCompare1(TIM5,183);     //8.04%
	}

}


