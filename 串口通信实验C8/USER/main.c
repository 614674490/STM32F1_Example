#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"

void My_USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);   //串口和GPIO口使能
	
	//GPIO端口模式设置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;    //PA9 TXD 推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    //PA10 RXD 浮空输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//串口初始化
	USART_InitStructure.USART_BaudRate=115200;               //设置波特率
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;    //不适用硬件流
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                      //使能发送和接受模式
	USART_InitStructure.USART_Parity=USART_Parity_No;                                //不使用奇偶校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                             //设置停止位 1
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                        //设置字长 8位
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);                                                        //使能串口1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);                                     //使能接受中断
	
	//开启中断分组初始化
	NVIC_InitInitStructure.NVIC_IRQChannel=USART1_IRQn;                              //串口1通道
	NVIC_InitInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitInitStructure.NVIC_IRQChannelPreemptionPriority=1;                      //抢占优先级为1
	NVIC_InitInitStructure.NVIC_IRQChannelSubPriority=1;                             //响应优先级为1
	NVIC_Init(&NVIC_InitInitStructure);
}

//编写中断处理函数
void USART1_IRQHandler(void)
{
	u16 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))                                   //判断是否发生接收中断
	{
		res=USART_ReceiveData(USART1);                                                 //接收数据
		if(res==0x01)                                             //在串口助手中药选择16进制发送，否则接收到的是文本形式，不能确定16进制数，还要关闭发送新行
			{
				led=0;
			}
			else if(res==0x00)
			{
				led=1;
			}
	  USART_SendData(USART1,res);                                                    //发送数据
	}
}
	
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //设置为第2组  2位抢占 2位响应
	My_USART1_Init();
	LED_Init();
	led=1;
	while(1)
	{
		//led=0;
	}
}


