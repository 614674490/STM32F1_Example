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
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);  //��ʼ������ģʽ ������ 500Kbps/s
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();
	KEY_Init();
	delay_init();
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)      //�����㰴�� ����һ������
		{
			printf("\r\nSend Data:");
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;
				printf("%d ",canbuf[i]);
			}
			printf("\r\n");
			res=Can_Send_Msg(canbuf,8);         //���Ͱ˸��ֽ�
			if(res)
				printf("\r\nSend Failed\r\n");
			else 
				printf("\r\nSend success\r\n");
		}
		else if(key==WKUP_PRES)    //�ı�CAN�Ĺ���ģʽ
		{
			mode=!mode;
			CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);  //��ʼ����ͨģʽ ������ 500Kbps/s
			printf("\r\nMODE:");
			if(mode==0) 
				printf("Nnormal Mode\r\n");           //��ͨģʽ
			else if(mode==1)
				printf("LoopBack Mode\r\n");     //����ģʽ
		}
		delay_ms(200);
		LED0=!LED0;
		cnt++;
	}
}


