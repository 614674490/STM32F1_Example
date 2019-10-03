#include "delay.h" 			 
#include "usart.h" 			 
#include "usart2.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"

//��ʼ��ATK-HC05ģ��
//����ֵ:0,�ɹ�;1,ʧ��.
u8 HC05_Init(void)
{
	u8 retry=10,t;
	u8 temp=1;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	//ʹ��PORTA Cʱ��	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��A4
	 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC4

	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	
//	HC05_KEY=1;
//	HC05_LED=1; 
 	
	USART2_Init(9600);	//��ʼ������2Ϊ:9600,������.
	
	while(retry--)
	{
		HC05_KEY=1;             //KEY�ø� ����ATָ��ģʽ
		delay_ms(10);
		u2_printf("AT\r\n");     //����AT����ָ��
		HC05_KEY=0;             //KEY���� �˳�ATָ��ģʽ
		for(t=0;t<10;t++)      //50ms�ȴ���Ӧ
		{
			if(USART2_RX_STA&0x8000) break;
			delay_ms(5);
		}
		if(USART2_RX_STA&0x8000)
		{
			temp=USART2_RX_STA&0x7fff;  //�õ����ݳ���
			USART2_RX_STA=0;
			if(temp==4&&USART2_RX_BUF[0]=='O'&&USART2_RX_BUF[1]=='K')
			{
				temp=0;              //�յ���Ӧ
				break;
			}
		}
	}
	if(retry==0) temp=1;
	return temp;
}

//��ȡATK-HC05ģ��Ľ�ɫ
//����ֵ:0,�ӻ�;1,����;0XFF,��ȡʧ��.							  
u8 HC05_Get_Role(void)
{
	u8 retry=0x0f,t;
	u8 temp;
	while(retry--)
	{
		HC05_KEY=1;             //KEY�ø� ����ATָ��ģʽ
		delay_ms(10);
		u2_printf("AT+ROLE?\r\n");     //����AT����ָ��
		HC05_KEY=0;             //KEY���� �˳�ATָ��ģʽ
		for(t=0;t<20;t++)      //200ms�ȴ���Ӧ
		{
			if(USART2_RX_STA&0x8000) break;
			delay_ms(10);
		}
		if(USART2_RX_STA&0x8000)
		{
			temp=USART2_RX_STA&0x7fff;  //�õ����ݳ���
			USART2_RX_STA=0;
			if(temp==13&&USART2_RX_BUF[0]=='+')     //+ROLE:0/1/2\r\nOK\r\n ��13���ַ� 0:�ӻ� 1:���� 2:�ػ���ɫ
			{
				temp=USART2_RX_BUF[6]-'0';              //�յ���Ӧ  ��������ASCALL 0x31-0x30=1
				break;
			}
		}
	}
	if(retry==0) temp=0XFF;                     //��ѯʧ��
	return temp;
}

//ATK-HC05��������
//�˺�����������ATK-HC05,�����ڽ�����OKӦ���ATָ��
//atstr:ATָ�.����:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"���ַ���
//����ֵ:0,���óɹ�;����,����ʧ��.							  
u8 HC05_Set_Cmd(u8* atstr)
{
	u8 retry=0x0f,t;
	u8 temp;
	while(retry--)
	{
		HC05_KEY=1;             //KEY�ø� ����ATָ��ģʽ
		delay_ms(10);
		u2_printf("%s\r\n",atstr);     //����AT����ָ��
		HC05_KEY=0;             //KEY���� �˳�ATָ��ģʽ
		for(t=0;t<20;t++)      //100ms�ȴ���Ӧ
		{
			if(USART2_RX_STA&0x8000) break;
			delay_ms(5);
		}
		if(USART2_RX_STA&0x8000)
		{
			temp=USART2_RX_STA&0x7fff;  //�õ����ݳ���
			USART2_RX_STA=0;
			if(temp==4&&USART2_RX_BUF[0]=='O')     //+ROLE:0/1/2\r\nOK\r\n ��13���ַ� 0:�ӻ� 1:���� 2:�ػ���ɫ
			{
				temp=0;              //�յ���Ӧ  ��������ASCALL 0x31-0x30=1
				break;
			}
		}
	}
	if(retry==0) temp=0XFF;                     //��ѯʧ��
	return temp;
}
			

