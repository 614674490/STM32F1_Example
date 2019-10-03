#ifndef _adc_h
#define _adc_h
#include "sys.h"
#include "stm32f10x_adc.h"

void Adc_Init(void);                     //ADC初始化
u16 Get_Adc(void);                      //获取ADC的值
u16 Get_Adc_Average(u8 times);     //多次获取，取平均值 
short Get_Temprate(void);         //转换成温度


#endif



