#include "delay.h" 			 
#include "usart.h" 			 
#include "usart2.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"

//初始化ATK-HC05模块
//返回值:0,成功;1,失败.
u8 HC05_Init(void)
{
	u8 retry=10,t;
	u8 temp=1;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	//使能PORTA C时钟	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化A4
	 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC4

	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	
//	HC05_KEY=1;
//	HC05_LED=1; 
 	
	USART2_Init(9600);	//初始化串口2为:9600,波特率.
	
	while(retry--)
	{
		HC05_KEY=1;             //KEY置高 进入AT指令模式
		delay_ms(10);
		u2_printf("AT\r\n");     //发送AT测试指令
		HC05_KEY=0;             //KEY拉低 退出AT指令模式
		for(t=0;t<10;t++)      //50ms等待回应
		{
			if(USART2_RX_STA&0x8000) break;
			delay_ms(5);
		}
		if(USART2_RX_STA&0x8000)
		{
			temp=USART2_RX_STA&0x7fff;  //得到数据长度
			USART2_RX_STA=0;
			if(temp==4&&USART2_RX_BUF[0]=='O'&&USART2_RX_BUF[1]=='K')
			{
				temp=0;              //收到响应
				break;
			}
		}
	}
	if(retry==0) temp=1;
	return temp;
}

//获取ATK-HC05模块的角色
//返回值:0,从机;1,主机;0XFF,获取失败.							  
u8 HC05_Get_Role(void)
{
	u8 retry=0x0f,t;
	u8 temp;
	while(retry--)
	{
		HC05_KEY=1;             //KEY置高 进入AT指令模式
		delay_ms(10);
		u2_printf("AT+ROLE?\r\n");     //发送AT测试指令
		HC05_KEY=0;             //KEY拉低 退出AT指令模式
		for(t=0;t<20;t++)      //200ms等待回应
		{
			if(USART2_RX_STA&0x8000) break;
			delay_ms(10);
		}
		if(USART2_RX_STA&0x8000)
		{
			temp=USART2_RX_STA&0x7fff;  //得到数据长度
			USART2_RX_STA=0;
			if(temp==13&&USART2_RX_BUF[0]=='+')     //+ROLE:0/1/2\r\nOK\r\n 共13个字符 0:从机 1:主机 2:回环角色
			{
				temp=USART2_RX_BUF[6]-'0';              //收到响应  数组存的是ASCALL 0x31-0x30=1
				break;
			}
		}
	}
	if(retry==0) temp=0XFF;                     //查询失败
	return temp;
}

//ATK-HC05设置命令
//此函数用于设置ATK-HC05,适用于仅返回OK应答的AT指令
//atstr:AT指令串.比如:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"等字符串
//返回值:0,设置成功;其他,设置失败.							  
u8 HC05_Set_Cmd(u8* atstr)
{
	u8 retry=0x0f,t;
	u8 temp;
	while(retry--)
	{
		HC05_KEY=1;             //KEY置高 进入AT指令模式
		delay_ms(10);
		u2_printf("%s\r\n",atstr);     //发送AT测试指令
		HC05_KEY=0;             //KEY拉低 退出AT指令模式
		for(t=0;t<20;t++)      //100ms等待回应
		{
			if(USART2_RX_STA&0x8000) break;
			delay_ms(5);
		}
		if(USART2_RX_STA&0x8000)
		{
			temp=USART2_RX_STA&0x7fff;  //得到数据长度
			USART2_RX_STA=0;
			if(temp==4&&USART2_RX_BUF[0]=='O')     //+ROLE:0/1/2\r\nOK\r\n 共13个字符 0:从机 1:主机 2:回环角色
			{
				temp=0;              //收到响应  数组存的是ASCALL 0x31-0x30=1
				break;
			}
		}
	}
	if(retry==0) temp=0XFF;                     //查询失败
	return temp;
}
			

