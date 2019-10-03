#ifndef __KEY_H
#define __KEY_H	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/11/28 
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.1修改说明
//修改按键扫描函数，使整个代码可以支持SWD下载。
//////////////////////////////////////////////////////////////////////////////////	 
typedef enum 
{  
	Button_KEY0 = 0,
	Button_KEY1 = 1,
	Button_WAKEUP = 2
} Button_TypeDef;

typedef struct  
{   GPIO_TypeDef*       BUTTON_PORT ;
    uint16_t            BUTTON_PIN ;
    GPIOMode_TypeDef    BUTTON_MODE;
}Button_PinModeDef;	

//#define KEY0 PAin(13)   //PA13
//#define KEY1 PAin(15)	//PA15 
//#define KEY2 PAin(0)	//PA0  WK_UP

#define IN1 PCin(11) //PC11
#define IN2 PCin(12) //PC12 
#define IN3 PDin(2)	 //PD2
#define IN4 PBin(3) //PB3
#define IN5 PBin(4) //PB4 
#define IN6 PBin(5)	 //PB5
#define IN7 PBin(6) //PB6
#define IN8 PBin(7) //PB7 
	 
void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);  //按键扫描函数					    
#endif
