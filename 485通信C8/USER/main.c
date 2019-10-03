#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "rs485.h"

int main(void)
{
	u8 key;
	u8 i=0;
	u8 cnt=0;
	u8 rs485buf[5];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	KEY_Init();
	LED_Init();
	delay_init();
	RS485_Init(9600);
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			printf("\r\nSend Data:");
			for(i=0;i<5;i++)
			{
				rs485buf[i]=cnt+i;
			  printf("%d",rs485buf[i]);
			}
			printf("\r\n");
			RS485_Send_Data(rs485buf,5);//发送5个字节 	
		}
		RS485_Receive_Data(rs485buf,&key);                   //时刻检测是否接收到数据
		if(key)                                            //如果接收到数据
		{
			printf("\r\nReceive Data:");
			for(i=0;i<5;i++)
				printf("%d",rs485buf[i]);       //显示接收到得数据
			printf("\r\n");
		}
		delay_ms(200);
		cnt++;
		led=!led;
	}
 	
}

