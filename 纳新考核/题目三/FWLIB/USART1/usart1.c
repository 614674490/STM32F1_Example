#include "usart1.h"
#include "stm32f10x_usart.h"
#include "led.h"
#include "stm32f10x_tim.h"
u8 USART1_RX_BUF[USART1_REC_LEN]; //数组用来存储接收到的数据，而USART3_REC_LEN为最多能接收的字节限度
u16 USART1_RX_STA=0;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位
//定时器3中断服务程序		    
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)    //产生更新中断
	{
		USART1_RX_STA|=1<<15;                             //标记接收完成
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);       //清除中断标志位
		TIM3_Set(0);                                      //关闭定时器3
	}
}
//设置TIM3的开关
//sta:0，关闭;1,开启;
void TIM3_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM3,0);//计数器清空
		TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
	}else TIM_Cmd(TIM3, DISABLE);//关闭定时器4	   
}

//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM3_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能//TIM3时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}
void My_USART1_Init(void)
{
	//GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//??USART1,GPIOA??
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.9
   
  //USART1_RX	  GPIOA.10???
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.10  

  //Usart1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
  
   //USART ?????

	USART_InitStructure.USART_BaudRate =115200;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

  USART_Init(USART1, &USART_InitStructure); //?????1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//????????
  USART_Cmd(USART1, ENABLE);                    //????1 
	//TIM3_Init(99,7199);		//10ms中断
	USART1_RX_STA=0;		//清零
	//TIM3_Set(0);			//关闭定时器3

}
//编写中断处理函数
/*void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)         //接收到数据
	{
		res=USART_ReceiveData(USART1);
		if(USART1_RX_STA<USART1_REC_LEN)                   //还能接收数据
		{
			TIM_SetCounter(TIM3,0);                                //清空计数器
			if(USART1_RX_STA==0) TIM3_Set(1);                    //使能接收中断
			USART1_RX_BUF[USART1_RX_STA&0x7fff]=res;                      //记录接收到的值
			USART1_RX_STA++;
		}
		else
			USART1_RX_STA|=1<<15;                               //超出最大字节 强制接收完成
	}
}*/
u16 len;
u8 flag=0;
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)         //接收到数据
	{
		res=USART_ReceiveData(USART1);
		if(!flag)
		{
			USART1_RX_BUF[USART1_RX_STA&0x7fff]=res;                      //记录接收到的值
			len=(++USART1_RX_STA)&0x7fff;
		}
		if(USART1_RX_BUF[0]!=0xaa)
			USART1_RX_STA=0;
		else
		{
			if(USART1_RX_BUF[len-1]==0x55)
			{
				USART1_RX_STA|=0x8000;
				flag=1;
			}
		}
		if((USART1_RX_STA&0x7fff)>USART1_REC_LEN)
			USART1_RX_STA|=0x8000;                        //强制接收完成
		USART_ClearITPendingBit(USART1,USART_IT_RXNE );
	}
}
			
		

