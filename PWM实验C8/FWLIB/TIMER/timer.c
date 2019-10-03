#include "timer.h"
#include "led.h"
#include "stm32f10x_tim.h"
/************ͨ�ö�ʱ��2��ʼ������*****************/
void TIM2_Int_Init(u16 arr,u16 psc)   //�Զ���װ�غ�Ԥ��Ƶϵ��
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //ʹ�ܶ�ʱ��2
	
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //TIM_CKD_DIV1  ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;            //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);         //ʹ�ܶ�ʱ��2�ĸ����ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);                               //ʹ�ܶ�ʱ��2
}
/*******************��д��ʱ��2�жϷ�����**************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)     //���TIM2�����ж��Ƿ���
	{
		relay=!relay;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);      //���TIM2���±�־λ
	}
}

/************TIM2 PWM���ֳ�ʼ������**************/
void TIM2_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO���� ����ʹ��AFIOʱ��

	//����������� ���TIM2_CH2��PWM����   PA1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;       //�����������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);   //���ò�����ӳ�� TIM2_CH2->PA1 
	//���ò�����ӳ����ʵ�֣���ΪӲ���ܽ������⣬�ö�Ӧ��IO��ʵ�־Ϳ���
	 
	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //TIM_CKD_DIV1  ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;            //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//��ʼ����ʱ��2ͨ��2������Ƚϲ���
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;             //PWMģʽ2
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;    //�ߵ�ƽ��Ч ���Ƚ��������Ϊ��
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);                     //�������ϲ�����ʼ��TIM2 OC2
	/*����������PWMģʽ2 ��CNT>CCRʱΪ�ߵ�ƽ С����Ϊ�͵�ƽ,CCRԽС,ռ�ձȾ�Խ�󣨸ߵ�ƽ����ʱ��Խ����*/
	
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);     //ʹ�ܶ�ʱ��2��CCR2��Ԥװ�ض�ʱ��
	
	TIM_Cmd(TIM2,ENABLE);                                //ʹ�ܶ�ʱ��2
}
	
	



