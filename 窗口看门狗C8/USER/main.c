#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "wdg.h"
 int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
	WWDG_Init(0x7f,0x5f,WWDG_Prescaler_8);         //Ԥ��Ƶϵ��Ϊ8
	led=0;
	//led1=1;
	while(1)
	{
    delay_ms(500);
	  led=1;      //��ʱ����Ϊ��һֱ������Ϊ500ms������ι��ʱ�䣬�����500ms֮ǰ��ϵͳ�Ѿ���λ���ʹ۲첻������
	}
}


