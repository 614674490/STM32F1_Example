#ifndef _dac_h
#define _dac_h
#include "sys.h"
#include "stm32f10x_dac.h"

void Dac1_Init(void);    //DAC1初始化
void Dac1_Set_Vol(u16 vol);   //电压与DAC数值转换




#endif


