#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //引用延时函数头文件
#include "key.h"
#include "oled.h"
#include "dma.h"
#include "usart.h"

#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.
u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
const u8 TEXT_TO_SEND[]={"ALIENTEK WarShip STM32F1 DMA 串口实验"};

int main()
{
	u16 i;
	u8 j,mask=0;
	u8 t=0;
	float pro=0;   //记录进度
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	LED_Init();
	delay_init();
	OLED_GPIO_Init();
	OLED_Init();
	KEY_Init();
	uart_init(115200);	 	//串口初始化为115200
	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);  //DMA1通道4 串口1地址 数组首地址 发送数据长度
	                             //加上&
	j=sizeof(TEXT_TO_SEND);
	for(i=0;i<SEND_BUF_SIZE;i++)
	{
		if(t>=j)                //添加回车换行符
		{
			if(mask)
			{
				SendBuff[i]=0x0a;
				t=0;
			}
			else
			{
				SendBuff[i]=0x0d;
				mask++;
			}
		}
		else
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];
			t++;
		}
	}
	i=0;
	while(1)
	{
		t=KEY_Scan(0);
		if(t==KEY0_PRES)//KEY0按下
		{
			OLED_P8x16Str(0,2,"Start Transimit");
			OLED_P8x16Str(0,5,"process:   %");
			printf("\r\nDMA DATA\r\n");
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);   ///使能串口1的DMA发送
			MYDMA_Enable(DMA1_Channel4);      //开始一次DMA传输  在传输期间 可以执行其他任务
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)    //判断通道四传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC4);
					break;
				}
				pro=DMA_GetCurrDataCounter(DMA1_Channel4);    //得到当前还剩下多少数据
				pro=1-pro/SEND_BUF_SIZE;
				pro=pro*100;                      //转换成百分比
				OLED_ShowNum(64,5,pro,3,16);
			}
			OLED_ShowNum(64,5,100,3,16);
			OLED_P8x16Str(0,2,"   Finished!   ");   //提示传输完成
		 }
			delay_ms(200);
			LED0=!LED0;                            //未闪烁表明传输未完成
	}
}
			
	
			
		
	
	
	



