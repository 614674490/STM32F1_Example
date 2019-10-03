#include "stm32f10x.h"
#include "sys.h"
#include "led.h"										  
////////////////////////////////////////////////////////////////////////////////// 	   
/*

#define LED PBout(8)

#define IN1 PAin(4)
#define IN2 PAin(5)
#define IN3 PAin(6)
#define IN4 PAin(7)
#define IN5 PBin(0)
#define IN6 PBin(1)
#define IN7 PBin(12)
#define IN8 PBin(13)
#define IN9 PBin(14)

#define OUT1 PBout(5)
#define OUT2 PBout(4)
#define OUT3 PBout(3)
#define OUT4 PAout(15) 
#define OUT5 PAout(14) 

#define RS485_READ_EN PCout(13)

*/
//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC\
//	|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);//使能端口时钟 复用管脚模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//使能端口时钟 复用管脚模式

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);

	//input-----------------------------------------------------------------------------
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
	//input-----------------------------------------------------------------------------

	//output----------------------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_14|GPIO_Pin_15);//=1

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8);//=1

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//=1

	//output----------------------------------------------------------------------------
}

