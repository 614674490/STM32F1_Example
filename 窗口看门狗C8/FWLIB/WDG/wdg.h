#ifndef _wdg_h
#define _wdg_h
#include "sys.h"
#include "stm32f10x_wwdg.h"

void WWDG_Init(u8 tr,u8 wr,u32 fprer);
void WWDG_IRQHander(void);



#endif

