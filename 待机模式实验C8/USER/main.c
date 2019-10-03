#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "wkup.h"
u8 i=0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)     //���TIM2�����ж��Ƿ���
	{
		i++;
		if(i==30)
		{
			Sys_Standby();                         //30s֮��������ģʽ
			i=0;
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);      //���TIM2���±�־λ
	}
}
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();
	delay_init();
	TIM2_Int_Init(9999,7199);     //1s
	//WKUP_Init();
	while(1)
	{
		led=0;
	}
	
}


