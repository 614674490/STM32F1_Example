#include "timer.h"

//TIM1����PWM�� �߼���ʱ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOA�ں�TIM1ʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;                            //���ù������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;                               //PA8->TIM1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;                 //ʱ�ӷָ�
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period=arr;                      //��װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                 //Ԥ��Ƶ ����Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	//pscԽ�� ռ�ձ�Խ��
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;          //PWM2ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;    //�͵�ƽ��Ч
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;    //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse=0;                          //���ô�װ�벶��ȽϼĴ�����ֵ
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Disable );      //CH1 Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);                      //ʹ��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);                      //MOE �����ʹ�� �߼���ʱ������������
	
	TIM_Cmd(TIM1,ENABLE);                                   //ʹ�ܶ�ʱ��1
}
//���������ѹ
//vol:0~330,����0~3.3V
void PWM_DAC_Set(u16 vol)
{
	float temp=vol;
	temp/=100;
	temp=temp*256/3.3;
	TIM_SetCompare1(TIM1,temp);
}

