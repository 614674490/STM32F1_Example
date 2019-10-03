#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "stm32f10x_exti.h"
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	KEY_Init();     //�����˿ڳ�ʼ��  ���ǳ�ʼ��IO��Ϊ����                             ��һ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //ʹ��AFIO�˿ڸ���ʱ��      �ڶ���
	/****************GPIOB4*************************************/
	/*****************����IO�����ж��ߵ�ӳ���ϵ************************/             //������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);  //��ӳ�䵽PA7
	
	/*****************���-��������ж�****************/                                 //���Ĳ�
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;    //�ж�������
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;    //ʹ��
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;    //ѡ���ⲿ�жϣ������¼��жϣ�
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;   //��������ģʽ �����½��ش��� ѡ�񴥷���ʽ
	EXTI_Init(&EXTI_InitStructure);
	
	/***********************�����жϷ���*****************************/
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;          //ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;           //ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;  //������ռ���ȼ�Ϊ2 ��ֵ���ܳ���2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;        //������Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);
	/**********************GPIOA1********************************/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);   //PA1
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;    //�ж�������
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //��������ģʽ ���������ش���
	EXTI_Init(&EXTI_InitStructure);                          //Ҫ����仰д��
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;          //ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;           //ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;  //������ռ���ȼ�Ϊ2 ��ֵ���ܳ���2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;        //������Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);
	
	
}
	
/******************��д�жϷ�����****************************/
void EXTI1_IRQHandler(void)     //  �ⲿ�ж�1�������
{
	delay_ms(50);   //��ʱ����
	if(WK_UP==1)
	{
		led=!led;
	}
	EXTI_ClearITPendingBit(EXTI_Line1);     //����жϱ�־λ
}

void EXTI9_5_IRQHandler(void)     //ǰ���voidҪ����   �ⲿ�ж�7�������
{
	delay_ms(50);   //��ʱ����
	if(KEY0==0)
	{
		led=1;
	}
	else led=0;
	EXTI_ClearITPendingBit(EXTI_Line7);     //����жϱ�־λ
}

