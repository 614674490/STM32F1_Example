#ifndef _dma_h
#define _dma_h
#include "sys.h"

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);  //通道配置函数
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);           //开启一次DMA传输

#endif 


