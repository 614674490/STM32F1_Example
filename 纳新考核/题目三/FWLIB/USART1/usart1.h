#ifndef USART1_H_
#define USART1_H_
#include "sys.h"
#include "stdio.h"	
#define USART1_REC_LEN  200  	//�����������ֽ��� 200
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�
extern u16 USART1_RX_STA;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
extern u16 len;
extern u8 flag;
void My_USART1_Init(void);
void TIM3_Init(u16 arr,u16 psc);
void TIM3_Set(u8 sta);



#endif

