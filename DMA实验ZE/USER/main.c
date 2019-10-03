#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //������ʱ����ͷ�ļ�
#include "key.h"
#include "oled.h"
#include "dma.h"
#include "usart.h"

#define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.
u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
const u8 TEXT_TO_SEND[]={"ALIENTEK WarShip STM32F1 DMA ����ʵ��"};

int main()
{
	u16 i;
	u8 j,mask=0;
	u8 t=0;
	float pro=0;   //��¼����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	LED_Init();
	delay_init();
	OLED_GPIO_Init();
	OLED_Init();
	KEY_Init();
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);  //DMA1ͨ��4 ����1��ַ �����׵�ַ �������ݳ���
	                             //����&
	j=sizeof(TEXT_TO_SEND);
	for(i=0;i<SEND_BUF_SIZE;i++)
	{
		if(t>=j)                //��ӻس����з�
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
		if(t==KEY0_PRES)//KEY0����
		{
			OLED_P8x16Str(0,2,"Start Transimit");
			OLED_P8x16Str(0,5,"process:   %");
			printf("\r\nDMA DATA\r\n");
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);   ///ʹ�ܴ���1��DMA����
			MYDMA_Enable(DMA1_Channel4);      //��ʼһ��DMA����  �ڴ����ڼ� ����ִ����������
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)    //�ж�ͨ���Ĵ������
				{
					DMA_ClearFlag(DMA1_FLAG_TC4);
					break;
				}
				pro=DMA_GetCurrDataCounter(DMA1_Channel4);    //�õ���ǰ��ʣ�¶�������
				pro=1-pro/SEND_BUF_SIZE;
				pro=pro*100;                      //ת���ɰٷֱ�
				OLED_ShowNum(64,5,pro,3,16);
			}
			OLED_ShowNum(64,5,100,3,16);
			OLED_P8x16Str(0,2,"   Finished!   ");   //��ʾ�������
		 }
			delay_ms(200);
			LED0=!LED0;                            //δ��˸��������δ���
	}
}
			
	
			
		
	
	
	



