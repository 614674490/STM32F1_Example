#ifndef _spi_h
#define _spi_h
#include "sys.h"
#include "stm32f10x_spi.h"

void SPI2_Init(void);   //SPI2的初始化函数
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);           //传输速度设置函数
u8 SPI2_ReadWriteByte(u8 TxData);                      //读写一个字节

#endif 

