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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
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
			RS485_Send_Data(rs485buf,5);//����5���ֽ� 	
		}
		RS485_Receive_Data(rs485buf,&key);                   //ʱ�̼���Ƿ���յ�����
		if(key)                                            //������յ�����
		{
			printf("\r\nReceive Data:");
			for(i=0;i<5;i++)
				printf("%d",rs485buf[i]);       //��ʾ���յ�������
			printf("\r\n");
		}
		delay_ms(200);
		cnt++;
		led=!led;
	}
 	
}

