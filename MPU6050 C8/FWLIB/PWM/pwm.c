#include "pwm.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
u32 dissz[8];//��������������
int dis;
u8 delayout;//�жϺ��������־

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure;  
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /* ��ʱ��2���� */                                            
    TIM_TimeBaseStructure.TIM_Period =arr; //��ֵ �������ֵ //50hz
    TIM_TimeBaseStructure.TIM_Prescaler = psc;   //��Ƶֵ       //50hz                          
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                              
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����ģʽΪ���ϼ���               
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    /* PWM1 ���� */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//pwm1ģʽ                       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = 0;                                  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High; 

	
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //A6
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC2Init(TIM3, &TIM_OCInitStructure); //A7
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ��� 
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //B0
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ��� 
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //B1
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR4�ϵ�Ԥװ�ؼĴ��� 
			/* TIM3 enable counter */  
		TIM_Cmd(TIM3, ENABLE);                                           
}//�������end

//***************************************************************************************************
//void tim4(void)//��ʱ��4��ʼ���������ɾ�ȷ��ʱ
//{
//	
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
//    /* Time base configuration */                                            
//		TIM_TimeBaseStructure.TIM_Period =0;  
//    TIM_TimeBaseStructure.TIM_Prescaler = 35999;                                    
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                              
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;               
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//	  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
// 
//��ʱ��1��ʼ������end
//}

void jqdelay(int delaytime)//��ʱ�����ɾ�ȷ��ʱ
{
  delayout=0;
	TIM4->ARR=delaytime*2;//�Ĵ���Ϊ��ʱ����ֵ
  TIM_Cmd(TIM4,ENABLE);
	while(delayout==0);//�ж��Ƿ�����ж�
	TIM_Cmd(TIM4,DISABLE);
	
}
int csbcj_w(void)//ǰ���������
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	delay_ms(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	
 	delay_ms(10);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//t�ڷ�������

		
  TIM1->CNT =0;//���tim3�������Ĵ���CNTΪ�����Ĵ���
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0);//�ȴ�e�����ݵ���
	TIM_Cmd(TIM1,ENABLE);//ʹ��tim3��ʼ����
  while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1)&&(TIM1->CNT<TIM1->ARR-10));//ARRΪ��ʱ����ֵ�Ĵ���
  TIM_Cmd(TIM1,DISABLE);//�ر�tim3
	dis=TIM1->CNT/58;
	//printf("sound return distance=%d\r\n",dis);
  return dis;
}

void tim1(void)//��ʱ��1��ʼ������ �洢����������
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
    /* Time base configuration */                                            
		TIM_TimeBaseStructure.TIM_Period =58000;  //������Ϊ��ֵ����58
    TIM_TimeBaseStructure.TIM_Prescaler =72;                                    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                              
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;               
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
		TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
}
void timconfig(void)//��ʱ����ʼ������
{
tim1();


}//��ʱ����ʼ������end


