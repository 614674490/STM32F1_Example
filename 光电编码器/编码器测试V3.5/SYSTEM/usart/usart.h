#ifndef __USART_H
#define __USART_H
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 

//#define USART1_START_BYTE 0xFF //
//#define USART1_END_BYTE   0x29 //')'
extern u8 usart1_buf[64];

extern u8 usart1_flag;

extern u8 usart1_count;

extern u8 usart1_step;

void usart1_init(u32 bound);
//void usart2_init(u32 bound);
//void usart3_init(u32 bound);


#endif
