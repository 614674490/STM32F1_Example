#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "led.h"
extern u8  TIM2CH2_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM2CH2_CAPTURE_VAL;	//���벶��ֵ

int main(void)
{
	u32 temp=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_Init();
	TIM2_Cap_Init(0xFFFF,72-1);   //��1MHZ���� һ��ʱ������Ϊ1us
	uart_init(115200);
	relay=0;
	while(1)
	{
		if(TIM2CH2_CAPTURE_STA&0x80)        //�ɹ�����һ��������
		{
			temp=TIM2CH2_CAPTURE_STA&0x3F;    //��¼�������
			temp=temp*65536;                  //ÿһ��һ�ξ���65536us
			temp+=TIM2CH2_CAPTURE_VAL;        //����ʣ��ߵ�ƽʱ��
			printf("HIGH:%d us\r\n",temp);    //����ߵ�ƽʱ��
			TIM2CH2_CAPTURE_STA=0;        //���²���������
		}
	}		
}









