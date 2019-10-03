#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define relay PCout(13)
#define led PCout(14)
void LED_Init(void);

#endif
