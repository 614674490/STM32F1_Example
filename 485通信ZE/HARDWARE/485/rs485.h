#ifndef _rs485_h
#define _rs485_h
#include "sys.h"

extern u8 RS485_RX_BUF[64];     //接受缓存 最大接收个字节
extern u8 RS485_RX_CNT;         //接收数据长度

#define RS485_TX_EN		PDout(7)	//485模式控制.0,接收;1,发送.  不要加分号

#define EN_USART2_RX 	1			//串口中断接收 0,不接收;1,接收.

void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);






#endif


