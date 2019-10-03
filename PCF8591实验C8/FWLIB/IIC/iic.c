#include "iic.h"
#include "delay.h"
#include "oled.h"
u32 ucReceDate;

void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    PBout(6)=1;
    PBout(7)=1;
}

/*����I^2C�ĺ���*/
/*I^2C��������*/
void IIC_Start()
{
	SDA=1;
	delay_us(1);
	SCL=1;
	delay_us(1);
	SDA=0;
	delay_us(1);
	SCL=0;  //�����½���
}
/*I2Cֹͣ����*/
void IIC_Stop()
{
	SDA=0;
	delay_us(1);
	SCL=1;
	delay_us(1);
	SDA=1;
	delay_us(1);  //����������
}
/*��Ӧ���ź�*/
void NOACK()
{
	SDA=1;
	delay_us(1);
	SCL=1;
	delay_us(1);
	SCL=0;
	delay_us(1);
	SDA=0;
	delay_us(1);
}
/*Ӧ���źź���*/
void ACK()
{
	SDA=0;
	delay_us(1);
	SCL=1;
	delay_us(1);
	SCL=0;
	delay_us(1);
	SDA=1;
	delay_us(1);
}
/*����һ�ֽ����ݺ���*/
void SendDate(u32 buffer)  //bufferΪ����ֵ
{
	u32 BitCnt=8;  //һ�ֽ�8λ
	u32 temp=0;  //�м����
	do
	{
		temp=buffer;
		SCL=0;
		delay_us(1);
		if((temp&0x80)==0)  //�ж����λ��0/1
			SDA=0;  //��������ֻ��0����1��ͨ���Ͼ���жϣ��ɽ����ݵĵ�һλд��SDA
		else
			SDA=1;
		delay_us(1);
		SCL=1;
		temp=buffer<<1;  //��buffer�е���������1����ȥ��λ����������д����һ����
		buffer=temp;  //�Ӹ�λ����λ����
		BitCnt--;
	}
	while(BitCnt);  //������8λ�ֽں�ֹͣ����
	SCL=0;
}
/*�����ֽں���*/
u8 ReceiveDate()
{
	u32 BitCnt=8;
	u32 temp=0;
	SDA=1;  //��������
	do
	{
		SCL=0;
		delay_us(1);
		SCL=1;
		delay_us(1);
		if(SDA==1)  //��һ������д�������
			ucReceDate=ucReceDate|0x01;  //��λ��1
		else
			ucReceDate=ucReceDate&0xfe;  //��λ��0
		if(BitCnt-1)
		{
			temp=ucReceDate<<1;  //��������1λ
			ucReceDate=temp;
		}
		BitCnt--;
	}
	while(BitCnt);
	SCL=0;
	return(ucReceDate);
}
void DACWrite(u8 sla,u8 c,u8 Val)  //д��D-Aת��ֵ ������ַ �����ֽ� ����
{
	IIC_Start();
	SendDate(sla);  //д��оƬ��ַ��д��
	ACK();  //Ӧ��
	SendDate(c);  //д�����λ
	ACK();  //Ӧ��
	SendDate(Val);  //д����
	ACK();  //Ӧ��
	IIC_Stop();
}
void ADCSend(u8 sla,u8 c)  //����������ַ�Ϳ���λ
{
	IIC_Start();
	SendDate(sla);  //д��оƬ��ַ��д��
	ACK();  //Ӧ��
	SendDate(c);  //д�����λ
	ACK();  //Ӧ��
	IIC_Stop();
}
u8 ADCRead (u8 sla)  //��ȡ����
{
	u8 date;
	IIC_Start();
	SendDate(sla+1);  //д��оƬ��ַ������
	ACK();  //Ӧ��
	date=ReceiveDate();
	NOACK();
	IIC_Stop();
	return(date);
}