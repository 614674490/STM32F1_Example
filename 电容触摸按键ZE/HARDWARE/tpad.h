#ifndef _tpad_h
#define _tpad_h
#include "sys.h"

void TIM5_Cap_Init(u16 arr,u16 psc);

u8 TAPD_Init(u16 psc);  //��ʼ����������
void TPAD_Reset(void);    //��λ����
u16 TPAD_Get_Val(void);    //��ó��ʱ��
u16 TPAD_Get_MaxVal(u8 n);   //��ȡn�λ�ó�����ʱ��
u8 TPAD_Scan(u8 mode);     //ɨ�败������

#endif


