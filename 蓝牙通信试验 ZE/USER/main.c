#include "sys.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "key.h"
#include "usart2.h"
#include "usart.h"
#include "hc05.h"
#include "string.h"	

//��ʾATK-HC05ģ�������״̬
void HC05_Role_Show(void)
{
	if(HC05_Get_Role()==1) LCD_ShowString(30,120,200,16,16,"ROLE:Master");	//����
	else LCD_ShowString(30,120,200,16,16,"ROLE:Slave ");	//����
}
//��ʾATK-HC05ģ�������״̬
void HC05_Sta_Show(void)
{
	if(HC05_LED) LCD_ShowString(30,140,200,16,16,"STA:Connected ");   //��ʾ���ӳɹ�
	else LCD_ShowString(30,140,200,16,16,"STA:Disconnect");   //��ʾδ����
}
int main()
{
	u8 t;                 //���ڶ�ʱ������Ϣ�ı���
	u8 key;                //����ɨ��ͱ�־λ 
	u8 sendmask=0;            //����ֹͣ��־λ
	u8 sendcnt=0;           //HC01/2/3/4/5......
	u8 sendbuf[20];       //�������ݻ������� ͨ��vxprintf() �洢������Ļ���������
	u8 reclen=0;          //��¼���յ������ݵĳ���
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
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
			if(sendmask==0) LCD_Fill(30+40,160,240,160+16,WHITE);   //���������ʾ
		}
		else delay_ms(10);
		if(t==50)
		{
			if(sendmask)                       //��ʱ����
			{
				sprintf((char*)sendbuf,"ALIENTEK HC05 %d\r\n",sendcnt);
				LCD_ShowString(30+40,160,200,16,16,sendbuf);	//��ʾ��������
				u2_printf("ALIENTEK HC05 %d\r\n",sendcnt);		//���͵�����ģ��
				sendcnt++;
				if(sendcnt>99) sendcnt=0;
				LED0=!LED0;
			}
			t=0;
			HC05_Sta_Show();
		}
		if(USART2_RX_STA&0x8000)         //���յ�һ������
		{
			LCD_Fill(30,200,240,320,WHITE);	//�����ʾ
			reclen=USART2_RX_STA&0x7fff;      //�õ����ݳ���
			USART2_RX_BUF[reclen]=0;          //���������
			if(reclen==9||reclen==8)
			{
				if(strcmp((const char*)USART2_RX_BUF,"+LED1 ON")==0) LED1=0;
				else if(strcmp((const char*)USART2_RX_BUF,"+LED1 OFF")==0) LED1=1;
			}
			LCD_ShowString(30,200,209,119,16,USART2_RX_BUF);//��ʾ���յ�������
			USART2_RX_STA=0;
		}
		t++;
	}
}


