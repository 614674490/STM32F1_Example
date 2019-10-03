#include "dma.h"

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从外设->存储器模式 /8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);      //使能DMA1时钟
	
	DMA_DeInit(DMA_CHx);                                  //将通道1的寄存器设为缺省值
	
	DMA1_MEM_LEN=cndtr;                                   	//把设置的值付给Cndtr
	
	DMA_InitStructure.DMA_BufferSize=cndtr;               //DMA通道的DMA缓存大小
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;     //数据传输方向 从外设到内存
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;            //关闭寄存器到寄存器模式
	DMA_InitStructure.DMA_MemoryBaseAddr=cmar;            //DMA内存基地址
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;   //数据宽度为32位   必须为32位，这里会影响到ADC的最大值！！
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;   //内存地址寄存器递增
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;             //工作在循环缓存模式 适用于多路ADC
	DMA_InitStructure.DMA_PeripheralBaseAddr=cpar;           //DMA外设基地址
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;    //数据宽度为32位
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;    //外设基地址寄存器不变
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;     //设置请求优先级 中
	DMA_Init(DMA_CHx,&DMA_InitStructure);
}

//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_Cmd(DMA_CHx,DISABLE);                            //关闭
	
	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);    //DMA通道DMA缓存大小
	
	DMA_Cmd(DMA_CHx,ENABLE);                           //使能
}




