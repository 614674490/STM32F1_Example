#ifndef _dma_h
#define _dma_h
#include "sys.h"

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);  //ͨ�����ú���
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);           //����һ��DMA����

#endif 


