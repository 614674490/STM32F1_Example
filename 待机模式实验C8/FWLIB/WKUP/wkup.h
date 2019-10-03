#ifndef _wkup_h
#define _wkup_h
#include "sys.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#define WKUP_KD PAin(0)         //���ڼ���ⲿ�����Ƿ���

void EXTI0_IRQHandler(void);   //�жϴ�����
void WKUP_Init(void);        //����ģʽ��ʼ��
u8 Check_WKUP(void);      //��⻽�Ѽ��ź�
void Sys_Enter_Standby(void);   //ϵͳ�������ģʽ
void Sys_Standby(void);        //����ģʽ

void TIM2_Int_Init(u16 arr,u16 psc);



#endif



