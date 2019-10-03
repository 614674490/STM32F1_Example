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
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);  //ÏßÓ³Éäµ½PA7
	
	/*****************³õÊ-¼»¯ÏßÉÏÖĞ¶Ï****************/                                 //µÚËÄ²½
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;    //ÖĞ¶ÏÏßÉèÖÃ
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
	/**********************GPIOA1********************************/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);   //PA1
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;    //ÖĞ¶ÏÏßÉèÖÃ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //¶ÔÓÚÏÂÀ­Ä£Ê½ ²ÉÓÃÉÏÉıÑØ´¥·¢
	EXTI_Init(&EXTI_InitStructure);                          //Òª°ÑÕâ¾ä»°Ğ´ÉÏ
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;          //Ñ¡ÔñÖĞ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;           //Ê¹ÄÜ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;  //ÉèÖÃÇÀÕ¼ÓÅÏÈ¼¶Îª2 ÆäÖµ²»ÄÜ³¬¹ı2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;        //ÉèÖÃÏìÓ¦ÓÅÏÈ¼¶Îª2
	NVIC_Init(&NVIC_InitStructure);
	
	
}
	
/******************±àĞ´ÖĞ¶Ï·şÎñº¯Êı****************************/
void EXTI1_IRQHandler(void)     //  Íâ²¿ÖĞ¶Ï1·şÎñ³ÌĞò
{
	delay_ms(50);   //ÑÓÊ±Ïû¶¶
	if(WK_UP==1)
	{
		led=!led;
	}
	EXTI_ClearITPendingBit(EXTI_Line1);     //Çå³şÖĞ¶Ï±êÖ¾Î»
}

void EXTI9_5_IRQHandler(void)     //Ç°ÃæµÄvoidÒª¼ÓÉÏ   Íâ²¿ÖĞ¶Ï7·şÎñ³ÌĞò
{
	delay_ms(50);   //ÑÓÊ±Ïû¶¶
	if(KEY0==0)
	{
		led=1;
	}
	else led=0;
	EXTI_ClearITPendingBit(EXTI_Line7);     //Çå³şÖĞ¶Ï±êÖ¾Î»
}

