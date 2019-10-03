#ifndef _spi_h
#define _spi_h
#include "sys.h"
#include "stm32f10x_spi.h"

void SPI2_Init(void);   //SPI2�ĳ�ʼ������
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);           //�����ٶ����ú���
u8 SPI2_ReadWriteByte(u8 TxData);                      //��дһ���ֽ�

#endif 

