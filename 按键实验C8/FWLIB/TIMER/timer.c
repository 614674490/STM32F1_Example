#include "timer.h"
#include "led.h"

void TIM5_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure
	GPIO_Init(&GPIO_InitStructure);
	