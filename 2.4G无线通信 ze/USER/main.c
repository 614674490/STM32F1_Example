#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h" 
#include "24l01.h" 	 
#include "usart.h"                       ///该程序有地方用到了串口 需要初始化串口
int main(void)
{
	u8 key,mode;
	u16 t,i;
	u8 tmp_buf[33];
	u8 buf[5]="led=0";
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	delay_init();
	LED_Init();
	LCD_Init();
	KEY_Init();
	NRF24L01_Init();
	POINT_COLOR=RED;			//设置字体为红色 
	while(NRF24L01_Check())
	{
		LCD_ShowString(30,130,200,16,16,"NRF24L01 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"NRF24L01 OK");
	while(1)                                     //模式选择
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			mode=0;
			break;
		}
		if(key==KEY1_PRES)
		{
			mode=1;
			break;
		}
		LCD_ShowString(10,150,230,16,16,"KEY0:RX_Mode  KEY1:TX_Mode"); 
	}
	LCD_Fill(10,150,240,166,WHITE);//清空上面的显示
  POINT_COLOR=BLUE;			//设置字体为蓝色	
	if(mode==0)                //接受模式
	{
		LCD_ShowString(10,150,200,16,16,"NRF24L01 RX_Mode"); 
		LCD_ShowString(30,170,200,16,16,"Received DATA:");	   
		NRF24L01_RX_Mode();
		while(1)
		{
			if(NRF24L01_RxPacket(tmp_buf)==0)                       //一收到信息就显示出来
			{
				tmp_buf[32]=0;                                           //加入字符串结束符
				LCD_ShowString(0,190,280,16,16,tmp_buf); 
			}
			delay_ms(200);
			LED0=!LED0;
	  }
	}
	else    //TX模式
	{
		LCD_ShowString(10,150,200,16,16,"NRF24L01 TX_Mode"); 
		LCD_ShowString(30,170,200,16,16,"Sended   DATA:");	   //显示发送的数据
		NRF24L01_TX_Mode();
		mode=' ';       //从空格键开始  
		while(1)
		{
			if(NRF24L01_TxPacket(buf)==TX_OK)
			{
				LCD_ShowString(0,190,280,16,16,buf);
//				LCD_ShowString(0,190,280,16,16,tmp_buf); 
//				key=mode;
//				for(t=0;t<32;t++)
//				{
//					key++;
//					if(key>('~')) key=' ';
//					tmp_buf[t]=key;
//				}
//				mode++;
//				if(mode>('~')) mode=' ';
//				tmp_buf[32]=0;                  //加入结束符
			}
			else
			{
				LCD_ShowString(30,170,200,16,16,"Sended   Failed");	   //显示发送的数据
			}
			delay_ms(500);
			LED1=!LED1;
		}
	}
}
		
	


