#ifndef _adc_h
#define _adc_h
#include "sys.h"
#include "stm32f10x_adc.h"

void Adc_Init(void);                     //ADC��ʼ��
u16 Get_Adc(u8 ch);                      //��ȡADC��ֵ
u16 Get_Adc_Average(u8 ch,u8 times);     //��λ�ȡ��ȡƽ��ֵ 


#endif



