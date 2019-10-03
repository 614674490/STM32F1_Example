#ifndef _gsm_h
#define _gsm_h
#include "sys.h"

u16 FindStr(u8 *sdat,u8 *tdat,u16 num,u16 num2);
u8 WaitForStr(u8 *p,u16 num,u8 timeout);
u8 GPS_ON(void);
u8 GPS_OFF(void);
u8 GPS_Init(void);
u8 A9G_Init(void);
u8 SIM_Connect(void);
u8 SendGPSData(u8 *dat);
void HttpGet(u8 *dat);
void Init(void);
void A9G_Check(void);


#endif

