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
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);  //��ӳ�䵽PB1
	
	/*****************���-��������ж�****************/                                 //���Ĳ�
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;    //�ж�������
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
	
}
	
/******************��д�жϷ�����****************************/
void EXTI9_5_IRQHandler(void)     //  �ⲿ�ж�1�������
{
	if(KEY0==0)
	{
		led=!led;
	}
	EXTI_ClearITPendingBit(EXTI_Line5);     //����жϱ�־λ
}


