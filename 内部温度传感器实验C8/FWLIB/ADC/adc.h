#ifndef _adc_h
#define _adc_h
#include "sys.h"
#include "stm32f10x_adc.h"

void Adc_Init(void);                     //ADC��ʼ��
u16 Get_Adc(void);                      //��ȡADC��ֵ
u16 Get_Adc_Average(u8 times);     //��λ�ȡ��ȡƽ��ֵ 
short Get_Temprate(void);         //ת�����¶�


#endif



