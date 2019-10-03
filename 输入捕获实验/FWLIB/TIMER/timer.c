#include "timer.h"
#include "led.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "usart.h"
void TIM2_Cap_Init(u16 arr,u16 psc)
{
	//TIM2_CH2->PA1
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	//��ʼ��IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;         //PA1 ��������  ���������ٶ�
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;             //PA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						       //PA1 ����
	
	//��ʼ����ʱ��2
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //����ʱ�ӷָ� TDTS=0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ü���ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period=arr;                      //�Զ�����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                   //Ԥ��Ƶ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//��ʼ�����벶��ͨ��
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;             //CC1S=01 ѡ������� IC2ӳ�䵽TI2�� ͨ��2
	TIM_ICInitStructure.TIM_ICFilter=0x00;                     //���������˲��� ���˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //���ò���ʽ �����ز���
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;        //���������Ƶ ����Ƶ
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI ;  //ѡ��ӳ�� ӳ�䵽TI2�� ͨ��2Ҫӳ�䵽ͨ��1ѡ���ӣ���ֱ֮��
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	//�жϳ�ʼ������
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;      //���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);      //��������жϺ�CC2E�����ж�
	
	TIM_Cmd(TIM2,ENABLE);                                   //ʹ�ܶ�ʱ��2
}

u32  TIM2CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM2CH2_CAPTURE_VAL;	//���벶��ֵ

void TIM2_IRQHandler(void)                     //��ʱ��2�жϴ�����
{
	if((TIM2CH2_CAPTURE_STA&0x80)==0)             //��δ�ɹ�����
	{
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)   //���������ж� ����65535 ����ж�
		{
			if(TIM2CH2_CAPTURE_STA&0x40)                  //����Ѿ����񵽸ߵ�ƽ
			{
				if((TIM2CH2_CAPTURE_STA&0x3F)==0x3F)          //����ߵ�ƽʱ��̫��
				{
					TIM2CH2_CAPTURE_STA|=0x80;                  //��ǳɹ�����ߵ�ƽһ��
					TIM2CH2_CAPTURE_VAL=0xFFFF;
				}
				else TIM2CH2_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)   //���������ж�  �ϴβ��񵽸ߵ�ƽ
		{
			if(TIM2CH2_CAPTURE_STA&0x40)                  //�������һ���½���
			{
				TIM2CH2_CAPTURE_STA|=0x80;                 //��ǳɹ�����һ�θ�����
				TIM2CH2_CAPTURE_VAL=TIM_GetCapture1(TIM2);
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);  //��Ϊ�����ز���
			}
			else                                            //��һ��δ���񵽸ߵ�ƽ ��һ�ο�ʼ����ߵ�ƽ
			{
				TIM2CH2_CAPTURE_STA=0;	//���		    				
      	TIM2CH2_CAPTURE_VAL=0;	
			  TIM_SetCounter(TIM2,0);         //��Ŀǰ��ʱ����ֵ����Ϊ0
				TIM2CH2_CAPTURE_STA|=0x40;      //��ǲ�����������
	      TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);  //��Ϊ�½��ز���
			}
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC2);     //����жϱ�־λ
}
	
				
	
	