#include "ds18b20.h"
#include "delay.h"

//复位脉冲
void DS18B20_Rst(void)
{
	DS18B20_IO_OUT();        //主机输出
	DS18B20_DQ_OUT=0;        //输出低电平
	delay_us(750);           //>=480
	DS18B20_DQ_OUT=1;        //释放总线
	delay_us(15);            //15~60
}

//应答脉冲
//返回值 0:应答 1:未应答
u8 DS18B20_Check(void)
{
	u8 retry=0;
	DS18B20_IO_IN();        //输入模式
	while(DS18B20_DQ_IN&&retry<200)    //200us 等待拉低总线
	{
		retry++;
		delay_us(1);
	};
	if(retry>=200) return 1;
	else retry=0;
	while(!DS18B20_DQ_IN&&retry<240)      //低电平持续时间60-240us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240) return 1;
  return 0;
}

//写一个字节
//dat:要写入的数据
void DS18B20_Write_Byte(u8 dat)
{
	u8 j;
	u8 testb;
	DS18B20_IO_OUT();              //主机输出
	for(j=1;j<=8;j++)
	{
		testb=dat&0x01;
		dat>>=1;
		if(testb)                 //写1
		{
			DS18B20_DQ_OUT=0;        //低电平
			delay_us(2);
			DS18B20_DQ_OUT=1;         //高电平
			delay_us(60);
		}
		else                       //写0
			{
			DS18B20_DQ_OUT=0;        //低电平
			delay_us(60);
			DS18B20_DQ_OUT=1;         //高电平
			delay_us(2);
		}
	}
}

//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(void) 
{
	u8 data;
	DS18B20_IO_OUT();          //主机输出
	DS18B20_DQ_OUT=0;          //低电平
	delay_us(2);
	DS18B20_DQ_OUT=1;           //释放总线
	DS18B20_IO_IN();             //主机输入
	delay_us(12);
	if(DS18B20_DQ_IN) data=1;    //读1
	else data=0;                 //读0
	delay_us(50);
	return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
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

//开始温度转换
void DS18B20_Start(void) 
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);        //发送SKIP RPM命令
	DS18B20_Write_Byte(0x44);        //发送开始转换命令
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PORTA口时钟 
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;               //PA0
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);    //输出1
	DS18B20_Rst();
	return DS18B20_Check();
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
	u8 temp;
	u8 TL,TH;
	short tem;
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);        //发送SKIP RPM命令
	DS18B20_Write_Byte(0xbe);        //发送存储器命令
	TL=DS18B20_Read_Byte();          //LSB
	TH=DS18B20_Read_Byte();          //MSB
	if(TH>7)                         //高五位为1 温度为负数
	{
		TH=~TH;
		TL=~TL;
		temp=0;                        //温度为负
	}
	else temp=1;                    //温度为正
	tem=TH;
	tem<<=8;
	tem+=TL;
	tem=(float)tem*0.625;         //按理应*0.0625 这里保留一位有效数字 故多*10
	if(temp) return tem;          //返回温度值
	else return -tem;
}




