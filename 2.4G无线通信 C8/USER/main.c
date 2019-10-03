#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "spi.h"
#include "usart.h"
#include "key.h"
#include "24l01.h"
#include "string.h"
 int main(void)
{
	u8 key,mode;
	u16 t;
	u8 tmp_buf[33];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	delay_init();
	LED_Init();
	KEY_Init();
	NRF24L01_Init();
	led=1;
	while(NRF24L01_Check())
	{
		printf("\rNRF24L01 Error");
	  delay_ms(200);
	}
	printf("\rNRF24L01 OK   ");
	printf("\r\nKEY0:RX_Mode  WK_UP:TX_Mode\r\n");
	while(1)                                     //模式选择
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			mode=0;
			break;
		}
		if(key==WK_UP_PRES)
		{
			mode=1;
			break;
		}
	}
	if(mode==0)                //接受模式
	{
		printf("\r\nNRF24L01 RX_Mode\r\n"); 
		printf("\r\nReceived DATA:\r\n");	   
		NRF24L01_RX_Mode();
		while(1)
		{
			if(NRF24L01_RxPacket(tmp_buf)==0)                       //一收到信息就显示出来
			{
				if(strcmp((const char*)tmp_buf,"led=0")==0) led=0;
				tmp_buf[5]=0;                                           //加入字符串结束符
				printf("\r\n%s\r\n",tmp_buf);
			}
	  }
	}
	else    //TX模式
	{
		printf("\r\nNRF24L01 TX_Mode\r\n"); 
		printf("\r\nSended   DATA:\r\n");	   //显示发送的数据
		NRF24L01_TX_Mode();
		mode=' ';       //从空格键开始  
		while(1)
		{
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				printf("\r\n%s",tmp_buf); 
				key=mode;
				for(t=0;t<32;t++)
				{
					key++;
					if(key>('~')) key=' ';
					tmp_buf[t]=key;
				}
				mode++;
				if(mode>('~')) mode=' ';
				tmp_buf[32]=0;                  //加入结束符
			}
			else
			{
				printf("\r\nSended   Failed\r\n");	   //显示发送的数据
			}
			delay_ms(500);
			led=1;
		}
	}
}
		
	



