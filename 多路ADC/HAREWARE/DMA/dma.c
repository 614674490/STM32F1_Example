#include "dma.h"

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//������->�洢��ģʽ /8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);      //ʹ��DMA1ʱ��
	
	DMA_DeInit(DMA_CHx);                                  //��ͨ��1�ļĴ�����Ϊȱʡֵ
	
	DMA1_MEM_LEN=cndtr;                                   	//�����õ�ֵ����Cndtr
	
	DMA_InitStructure.DMA_BufferSize=cndtr;               //DMAͨ����DMA�����С
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;     //���ݴ��䷽�� �����赽�ڴ�
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;            //�رռĴ������Ĵ���ģʽ
	DMA_InitStructure.DMA_MemoryBaseAddr=cmar;            //DMA�ڴ����ַ
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;   //���ݿ��Ϊ32λ   ����Ϊ32λ�������Ӱ�쵽ADC�����ֵ����
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;   //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;             //������ѭ������ģʽ �����ڶ�·ADC
	DMA_InitStructure.DMA_PeripheralBaseAddr=cpar;           //DMA�������ַ
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;    //���ݿ��Ϊ32λ
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;    //�������ַ�Ĵ�������
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;     //�����������ȼ� ��
	DMA_Init(DMA_CHx,&DMA_InitStructure);
}

//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_Cmd(DMA_CHx,DISABLE);                            //�ر�
	
	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);    //DMAͨ��DMA�����С
	
	DMA_Cmd(DMA_CHx,ENABLE);                           //ʹ��
}




