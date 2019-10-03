#include "sys.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "key.h"
#include "usart2.h"
#include "usart.h"
#include "hc05.h"
#include "string.h"	

//显示ATK-HC05模块的主从状态
void HC05_Role_Show(void)
{
	if(HC05_Get_Role()==1) LCD_ShowString(30,120,200,16,16,"ROLE:Master");	//主机
	else LCD_ShowString(30,120,200,16,16,"ROLE:Slave ");	//主机
}
//显示ATK-HC05模块的连接状态
void HC05_Sta_Show(void)
{
	if(HC05_LED) LCD_ShowString(30,140,200,16,16,"STA:Connected ");   //显示连接成功
	else LCD_ShowString(30,140,200,16,16,"STA:Disconnect");   //显示未连接
}
int main()
{
	u8 t;                 //用于定时发送信息的变量
	u8 key;                //按键扫描和标志位 
	u8 sendmask=0;            //发送停止标志位
	u8 sendcnt=0;           //HC01/2/3/4/5......
	u8 sendbuf[20];       //发送数据缓存数组 通过vxprintf() 存储到定义的缓存数组中
	u8 reclen=0;          //记录接收到的数据的长度
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为9600
	USART2_Init(9600);
	KEY_Init();
	LCD_Init();
	LED_Init();
	POINT_COLOR=RED;
	LCD_ShowString(30,70,200,16,16,"KEY0:SEND/STOP");   
	LCD_ShowString(30,160,200,16,16,"Send:");	
	LCD_ShowString(30,180,200,16,16,"Receive:");
	POINT_COLOR=BLUE;
	HC05_Role_Show();
	while(1)
	{
		key=KEY_Scan(0);
	  if(key==KEY0_PRES)
		{
			sendmask=!sendmask;
			if(sendmask==0) LCD_Fill(30+40,160,240,160+16,WHITE);   //清除数据显示
		}
		else delay_ms(10);
		if(t==50)
		{
			if(sendmask)                       //定时发送
			{
				sprintf((char*)sendbuf,"ALIENTEK HC05 %d\r\n",sendcnt);
				LCD_ShowString(30+40,160,200,16,16,sendbuf);	//显示发送数据
				u2_printf("ALIENTEK HC05 %d\r\n",sendcnt);		//发送到蓝牙模块
				sendcnt++;
				if(sendcnt>99) sendcnt=0;
				LED0=!LED0;
			}
			t=0;
			HC05_Sta_Show();
		}
		if(USART2_RX_STA&0x8000)         //接收到一次数据
		{
			LCD_Fill(30,200,240,320,WHITE);	//清除显示
			reclen=USART2_RX_STA&0x7fff;      //得到数据长度
			USART2_RX_BUF[reclen]=0;          //加入结束符
			if(reclen==9||reclen==8)
			{
				if(strcmp((const char*)USART2_RX_BUF,"+LED1 ON")==0) LED1=0;
				else if(strcmp((const char*)USART2_RX_BUF,"+LED1 OFF")==0) LED1=1;
			}
			LCD_ShowString(30,200,209,119,16,USART2_RX_BUF);//显示接收到的数据
			USART2_RX_STA=0;
		}
		t++;
	}
}


