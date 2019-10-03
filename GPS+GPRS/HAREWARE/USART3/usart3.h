#ifndef _usart3_h
#define _usart3_h
#include "sys.h"

#define USART3_MAX_RECV_LEN		1000					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		1000					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   						//��������״̬

void USART3_Init(u32 bound);				//����3��ʼ�� 
void TIM3_Set(u8 sta);
void TIM3_Init(u16 arr,u16 psc);


#endif
