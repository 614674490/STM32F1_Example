#ifndef _key_h
#define _key_h
#include "sys.h"
/******λ����*************/
/*#define KEY0 PAin(7)
#define WK_UP PAin(1)*/
/****************�⺯��******************/
#define KEY0 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)     //��ȡ����0
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)     //��ȡ����1

#define KEY0_PRES 1      //����0���£�����1
#define WK_UP_PRES 2     //����1���·���2

void KEY_Init(void);     //IO�ڳ�ʼ��
u8 KEY_Scan(u8 mode);    //����ɨ�躯��

#endif

