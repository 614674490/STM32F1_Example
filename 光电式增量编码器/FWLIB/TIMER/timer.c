#include "timer.h"
#include "led.h"
#include "stm32f10x.h"
#include "usart.h"
#include "stm32f10x_tim.h"
void TIM3_Encoder_Init(u16 arr,u16 psc)    //������ģʽ��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	//��ʼ��IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;              //��������  ��ʱPA6 7Ϊ�ߵ�ƽ �ȴ���Ϊ�͵�ƽ       
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;         
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ����ʱ��3
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //����ʱ�ӷָ� TDTS=0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ü���ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period=arr;                      //�Զ�����ֵ  �Ķ�
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                   //Ԥ��Ƶ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//������ģʽ3 �½��ش���
	TIM_ICStructInit(&TIM_ICInitStructure);   //����Ĭ�����ò���
	TIM_ICInitStructure.TIM_ICFilter=0x00;                     //���������˲��� ���˲�
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//�жϳ�ʼ������
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;      //���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);      //�����־λ         //����жϱ�־λ
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);      //��������ж�    //ʹ���ж�
	
	TIM_Cmd(TIM3,ENABLE);                                   //ʹ�ܶ�ʱ��3
}




	
				
	
	