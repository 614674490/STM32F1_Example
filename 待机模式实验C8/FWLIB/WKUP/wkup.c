#include "wkup.h"
#include "led.h"
#include "delay.h"
//����ģʽ
void Sys_Standby(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);   //ʹ�ܵ�Դʱ��
	PWR_WakeUpPinCmd(ENABLE);                           //����WK_UP������Ϊ����Դ
	PWR_EnterSTANDBYMode();                            //�������ģʽ
}

//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	Sys_Standby();
}

//���WKUP�ŵ��ź�
//����ֵ1:��������3s����
//      0:����Ĵ���	
u8 Check_WKUP(void) 
{
	u8 t=0;
	while(1)
	{
		if(WKUP_KD)
		{
			t++;
			delay_ms(30);
			if(t>=100)                 //�������³���������
			{
				return 1;
			}
		}
			else
			{
				return 0;                    //���򷵻�1
			}
		}
}

//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);   //ʹ��GPIOA��AFIO
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;    //��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//�ⲿ�ж�0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	//���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	if(Check_WKUP()==0)  Sys_Enter_Standby();   //�տ���ʱ��δ��������ֱ�ӽ������ģʽ
}

//�жϴ����� δ�������ģʽǰ WK_UPΪ��ͨIO�� ��ͨ���ж����������ģʽ ������֮�� �ü�Ϊ���Ѽ�
void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);                  //����жϱ�־λ
	if(Check_WKUP())    //Ϊ1���ʾ�������״̬
		Sys_Enter_Standby();
}

void TIM2_Int_Init(u16 arr,u16 psc)   //�Զ���װ�غ�Ԥ��Ƶϵ��
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //ʹ�ܶ�ʱ��2
	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
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


