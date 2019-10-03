#include "ds18b20.h"
#include "delay.h"

//��λ����
void DS18B20_Rst(void)
{
	DS18B20_IO_OUT();        //�������
	DS18B20_DQ_OUT=0;        //����͵�ƽ
	delay_us(750);           //>=480
	DS18B20_DQ_OUT=1;        //�ͷ�����
	delay_us(15);            //15~60
}

//Ӧ������
//����ֵ 0:Ӧ�� 1:δӦ��
u8 DS18B20_Check(void)
{
	u8 retry=0;
	DS18B20_IO_IN();        //����ģʽ
	while(DS18B20_DQ_IN&&retry<200)    //200us �ȴ���������
	{
		retry++;
		delay_us(1);
	};
	if(retry>=200) return 1;
	else retry=0;
	while(!DS18B20_DQ_IN&&retry<240)      //�͵�ƽ����ʱ��60-240us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240) return 1;
  return 0;
}

//дһ���ֽ�
//dat:Ҫд�������
void DS18B20_Write_Byte(u8 dat)
{
	u8 j;
	u8 testb;
	DS18B20_IO_OUT();              //�������
	for(j=1;j<=8;j++)
	{
		testb=dat&0x01;
		dat>>=1;
		if(testb)                 //д1
		{
			DS18B20_DQ_OUT=0;        //�͵�ƽ
			delay_us(2);
			DS18B20_DQ_OUT=1;         //�ߵ�ƽ
			delay_us(60);
		}
		else                       //д0
			{
			DS18B20_DQ_OUT=0;        //�͵�ƽ
			delay_us(60);
			DS18B20_DQ_OUT=1;         //�ߵ�ƽ
			delay_us(2);
		}
	}
}

//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 
{
	u8 data;
	DS18B20_IO_OUT();          //�������
	DS18B20_DQ_OUT=0;          //�͵�ƽ
	delay_us(2);
	DS18B20_DQ_OUT=1;           //�ͷ�����
	DS18B20_IO_IN();             //��������
	delay_us(12);
	if(DS18B20_DQ_IN) data=1;    //��1
	else data=0;                 //��0
	delay_us(50);
	return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void) 
{
	u8 i,j,dat=0;
	for(i=0;i<8;i++)
	{
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
	}
	return dat;
}

//��ʼ�¶�ת��
void DS18B20_Start(void) 
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);        //����SKIP RPM����
	DS18B20_Write_Byte(0x44);        //���Ϳ�ʼת������
}

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PORTA��ʱ�� 
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;               //PA0
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);    //���1
	DS18B20_Rst();
	return DS18B20_Check();
}

//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
	u8 temp;
	u8 TL,TH;
	short tem;
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);        //����SKIP RPM����
	DS18B20_Write_Byte(0xbe);        //���ʹ洢������
	TL=DS18B20_Read_Byte();          //LSB
	TH=DS18B20_Read_Byte();          //MSB
	if(TH>7)                         //����λΪ1 �¶�Ϊ����
	{
		TH=~TH;
		TL=~TL;
		temp=0;                        //�¶�Ϊ��
	}
	else temp=1;                    //�¶�Ϊ��
	tem=TH;
	tem<<=8;
	tem+=TL;
	tem=(float)tem*0.625;         //����Ӧ*0.0625 ���ﱣ��һλ��Ч���� �ʶ�*10
	if(temp) return tem;          //�����¶�ֵ
	else return -tem;
}




