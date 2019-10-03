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

/*关于I^2C的函数*/
/*I^2C启动函数*/
void IIC_Start()
{
	SDA=1;
	delay_us(1);
	SCL=1;
	delay_us(1);
	SDA=0;
	delay_us(1);
	SCL=0;  //产生下降沿
}
/*I2C停止函数*/
void IIC_Stop()
{
	SDA=0;
	delay_us(1);
	SCL=1;
	delay_us(1);
	SDA=1;
	delay_us(1);  //产生上升沿
}
/*非应答信号*/
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
/*应答信号函数*/
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
/*发送一字节数据函数*/
void SendDate(u32 buffer)  //buffer为输入值
{
	u32 BitCnt=8;  //一字节8位
	u32 temp=0;  //中间变量
	do
	{
		temp=buffer;
		SCL=0;
		delay_us(1);
		if((temp&0x80)==0)  //判断最高位是0/1
			SDA=0;  //由于数据只有0或者1，通过上句的判断，可将数据的第一位写入SDA
		else
			SDA=1;
		delay_us(1);
		SCL=1;
		temp=buffer<<1;  //将buffer中的数据左移1，除去高位的数，继续写入下一个数
		buffer=temp;  //从高位到低位发送
		BitCnt--;
	}
	while(BitCnt);  //发送完8位字节后停止发送
	SCL=0;
}
/*接收字节函数*/
u8 ReceiveDate()
{
	u32 BitCnt=8;
	u32 temp=0;
	SDA=1;  //读入数据
	do
	{
		SCL=0;
		delay_us(1);
		SCL=1;
		delay_us(1);
		if(SDA==1)  //上一个函数写入的数据
			ucReceDate=ucReceDate|0x01;  //低位置1
		else
			ucReceDate=ucReceDate&0xfe;  //低位置0
		if(BitCnt-1)
		{
			temp=ucReceDate<<1;  //数据左移1位
			ucReceDate=temp;
		}
		BitCnt--;
	}
	while(BitCnt);
	SCL=0;
	return(ucReceDate);
}
void DACWrite(u8 sla,u8 c,u8 Val)  //写入D-A转换值 器件地址 控制字节 数据
{
	IIC_Start();
	SendDate(sla);  //写入芯片地址（写）
	ACK();  //应答
	SendDate(c);  //写入控制位
	ACK();  //应答
	SendDate(Val);  //写数据
	ACK();  //应答
	IIC_Stop();
}
void ADCSend(u8 sla,u8 c)  //发送器件地址和控制位
{
	IIC_Start();
	SendDate(sla);  //写入芯片地址（写）
	ACK();  //应答
	SendDate(c);  //写入控制位
	ACK();  //应答
	IIC_Stop();
}
u8 ADCRead (u8 sla)  //读取数据
{
	u8 date;
	IIC_Start();
	SendDate(sla+1);  //写入芯片地址（读）
	ACK();  //应答
	date=ReceiveDate();
	NOACK();
	IIC_Stop();
	return(date);
}