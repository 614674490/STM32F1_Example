#include "wdg.h"
#include "led.h"
#include "delay.h"
//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 
//��ʼ�����ڿ��Ź� 	
//tr   :T[6:0],������ֵ 
//wr   :W[6:0],����ֵ 
//fprer:��Ƶϵ����WDGTB��,�����2λ��Ч 
//Fwwdg=PCLK1/(4096*2^fprer). 
void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);           //ʹ��ʱ��
	
	WWDG_CNT=tr&WWDG_CNT;
	WWDG_SetPrescaler(fprer);                                   	//���÷�Ƶϵ��
	WWDG_SetWindowValue(wr);                                      //���ô���ֵ
	WWDG_Enable(WWDG_CNT);                                       //ʹ�ܿ��Ź������ó�ֵ
	
	//�жϳ�ʼ������
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ2�������ȼ�3����2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //��ռ2�������ȼ�3����2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
	
	WWDG_ClearFlag();                                      //����жϱ�־λ
	
	WWDG_EnableIT();                                       //ʹ���жϱ�־λ
}

//��д�жϴ�����
void WWDG_IRQHander() 
{
	WWDG_SetCounter(WWDG_CNT);                            //����ι��
	WWDG_ClearFlag();                                     //����жϱ�־λ
	led1=0;
}


