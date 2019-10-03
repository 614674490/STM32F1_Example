#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#include "sys.h"

//端口定义

#define LED PBout(8)

#define IN1 PAin(4)
#define IN2 PAin(5)
//#define IN3 PAin(6)
//#define IN4 PAin(7)
#define IN3 PBin(0)
#define IN4 PBin(1)
#define IN5 PBin(12)
#define IN6 PBin(13)
#define IN7 PBin(14)

#define OUT1 PBout(5) // 
#define OUT2 PBout(4) // 
#define OUT3 PBout(3) // 
#define OUT4 PAout(15) // 
#define OUT5 PAout(14) // 

#define RS485_READ_EN PCout(13) //0 is USART2_READ_MODE

void LED_Init(void);//初始化
	 				    
#endif
