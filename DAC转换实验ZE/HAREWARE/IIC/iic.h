#ifndef _iic_h
#define _iic_h
#include "sys.h"

#define SCL PBout(6) //串行时钟
#define SDA PBout(7) //串行数据

void IIC_Start(void);
void IIC_Stop(void);
void NOACK(void);
void ACK(void);
void SendDate(u32 buffer);
u8 ReceiveDate(void);
void DACWrite(u8 sla,u8 c,u8 Val);
void ADCSend(u8 sla,u8 c);
u8 ADCRead (u8 sla);
void IIC_Init(void);





#endif
