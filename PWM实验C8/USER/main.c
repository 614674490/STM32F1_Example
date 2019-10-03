#include "stm32f10x_tim.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "timer.h"
 int main(void)
{
	u16 ledpwmval=0;
	u8  flag=1;                 //�˴�����Ҫ������ǰ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	//LED_Init();
	TIM2_PWM_Init(2500,7199);     //20ms
	//relay=1;    //�ȹر�LED
	while(1)
	{
		delay_ms(10);
		if(flag) ledpwmval++;
		else ledpwmval--;
		
		if(ledpwmval==0) flag=1;
		else if(ledpwmval>190) flag=0;
		
		TIM_SetCompare2(TIM2,1000);     //8.04%
	}
}
		


