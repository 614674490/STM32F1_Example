#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h" 
#include "24l01.h" 	 
#include "usart.h"                       ///�ó����еط��õ��˴��� ��Ҫ��ʼ������
int main(void)
{
	u8 key,mode;
	u16 t,i;
	u8 tmp_buf[33];
	u8 buf[5]="led=0";
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	delay_init();
	LED_Init();
	LCD_Init();
	KEY_Init();
	NRF24L01_Init();
	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	while(NRF24L01_Check())
	{
		LCD_ShowString(30,130,200,16,16,"NRF24L01 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"NRF24L01 OK");
	while(1)                                     //ģʽѡ��
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
	LCD_Fill(10,150,240,166,WHITE);//����������ʾ
  POINT_COLOR=BLUE;			//��������Ϊ��ɫ	
	if(mode==0)                //����ģʽ
	{
		LCD_ShowString(10,150,200,16,16,"NRF24L01 RX_Mode"); 
		LCD_ShowString(30,170,200,16,16,"Received DATA:");	   
		NRF24L01_RX_Mode();
		while(1)
		{
			if(NRF24L01_RxPacket(tmp_buf)==0)                       //һ�յ���Ϣ����ʾ����
			{
				tmp_buf[32]=0;                                           //�����ַ���������
				LCD_ShowString(0,190,280,16,16,tmp_buf); 
			}
			delay_ms(200);
			LED0=!LED0;
	  }
	}
	else    //TXģʽ
	{
		LCD_ShowString(10,150,200,16,16,"NRF24L01 TX_Mode"); 
		LCD_ShowString(30,170,200,16,16,"Sended   DATA:");	   //��ʾ���͵�����
		NRF24L01_TX_Mode();
		mode=' ';       //�ӿո����ʼ  
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
//				tmp_buf[32]=0;                  //���������
			}
			else
			{
				LCD_ShowString(30,170,200,16,16,"Sended   Failed");	   //��ʾ���͵�����
			}
			delay_ms(500);
			LED1=!LED1;
		}
	}
}
		
	


