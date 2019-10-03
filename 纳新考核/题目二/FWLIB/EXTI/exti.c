#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "stm32f10x_exti.h"
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	KEY_Init();     //°´¼ü¶Ë¿Ú³õÊ¼»¯  ¾ÍÊÇ³õÊ¼»¯IO¿ÚÎªÊäÈë                             µÚÒ»²½
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //Ê¹ÄÜAFIO¶Ë¿Ú¸´ÓÃÊ±ÖÓ      µÚ¶ş²½
	/****************GPIOB4*************************************/
	/*****************ÉèÖÃIO¿ÚÓëÖĞ¶ÏÏßµÄÓ³Éä¹ØÏµ************************/             //µÚÈı²½
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);  //ÏßÓ³Éäµ½PB1
	
	/*****************³õÊ-¼»¯ÏßÉÏÖĞ¶Ï****************/                                 //µÚËÄ²½
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;    //ÖĞ¶ÏÏßÉèÖÃ
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;    //Ê¹ÄÜ
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;    //Ñ¡ÔñÍâ²¿ÖĞ¶Ï£¨»¹ÓĞÊÂ¼şÖĞ¶Ï£©
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;   //¶ÔÓÚÉÏÀ­Ä£Ê½ ²ÉÓÃÏÂ½µÑØ´¥·¢ Ñ¡Ôñ´¥·¢·½Ê½
	EXTI_Init(&EXTI_InitStructure);
	
	/***********************ÅäÖÃÖĞ¶Ï·Ö×é*****************************/
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;          //Ñ¡ÔñÖĞ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;           //Ê¹ÄÜ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;  //ÉèÖÃÇÀÕ¼ÓÅÏÈ¼¶Îª2 ÆäÖµ²»ÄÜ³¬¹ı2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;        //ÉèÖÃÏìÓ¦ÓÅÏÈ¼¶Îª2
	NVIC_Init(&NVIC_InitStructure);
	
}
	
/******************±àĞ´ÖĞ¶Ï·şÎñº¯Êı****************************/
void EXTI9_5_IRQHandler(void)     //  Íâ²¿ÖĞ¶Ï1·şÎñ³ÌĞò
{
	if(KEY0==0)
	{
		led=!led;
	}
	EXTI_ClearITPendingBit(EXTI_Line5);     //Çå³şÖĞ¶Ï±êÖ¾Î»
}


