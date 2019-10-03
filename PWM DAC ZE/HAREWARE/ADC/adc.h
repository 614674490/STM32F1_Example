#ifndef _adc_h
#define _adc_h
#include "sys.h"
#include "stm32f10x_adc.h"

void Adc_Init(void);                     //ADC初始化
u16 Get_Adc(u8 ch);                      //获取ADC的值
u16 Get_Adc_Average(u8 ch,u8 times);     //多次获取，取平均值 


#endif



