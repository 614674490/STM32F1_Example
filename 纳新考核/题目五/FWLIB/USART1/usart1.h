#ifndef USART1_H_
#define USART1_H_
#include "sys.h"
#include "stdio.h"	
#define USART1_REC_LEN  200  	//�����������ֽ��� 200
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�
extern u16 USART1_RX_STA;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
void My_USART1_Init(void);



#endif

