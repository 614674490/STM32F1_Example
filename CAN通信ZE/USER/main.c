#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "can.h"
#include "key.h"
int main()
{
	u8 key;
	u8 i=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 res;
	u8 mode=CAN_Mode_Normal;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);  //初始化环回模式 波特率 500Kbps/s
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	KEY_Init();
	delay_init();
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)      //按键零按下 发送一次数据
		{
			printf("\r\nSend Data:");
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;
				printf("%d ",canbuf[i]);
			}
			printf("\r\n");
			res=Can_Send_Msg(canbuf,8);         //发送八个字节
			if(res)
				printf("\r\nSend Failed\r\n");
			else 
				printf("\r\nSend success\r\n");
		}
		else if(key==WKUP_PRES)    //改变CAN的工作模式
		{
			mode=!mode;
			CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);  //初始化普通模式 波特率 500Kbps/s
			printf("\r\nMODE:");
			if(mode==0) 
				printf("Nnormal Mode\r\n");           //普通模式
			else if(mode==1)
				printf("LoopBack Mode\r\n");     //环回模式
		}
		delay_ms(200);
		LED0=!LED0;
		cnt++;
	}
}


