#include "Encoder.h"
///////////////////////////////////////////////////////
//����:2015.10.11
//�ص㣺��ͷ��ѧ
//���ߣ�Ҧ��

//////////////////////////////////////////////////////
void Encoder_Init(void)
{
  GPIO_InitTypeDef         GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//����TIM2ʱ��
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//����GPIOA��ʱ�� 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);//����GPIOA0��TIM2ʱ������
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);//����GPIOA1��TIM2ʱ������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //GPIOA0,GPIOA1
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//���ö˿�������� 0 ������죨��λ״̬��  1�����©
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;//���ö˿����������ļĴ�����00������������01������10 ���� 11����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA0  PA1

	
  TIM_TimeBaseStructure.TIM_Period = 60000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	//���ö�ʱ��2Ϊ������ģʽ  IT1 IT2Ϊ�����ؼ���
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;  //�����˲���
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);  //������б�־λ
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //�����жϸ���
  TIM3->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}






























