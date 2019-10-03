#include "spi.h"


void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);       //使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               //复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //SCLK MISO MOSI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为256,数越小，传输速度就越快
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                         //串行同步时钟第二个跳边沿进行数据采样
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                          //串行同步时钟空闲状态位高电平
	SPI_InitStructure.SPI_CRCPolynomial=7;                             //CRC值计算的多项式 用于校准
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                    //选择数据大小 SPI发送接收8位字节
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;   //选择单向或者双向的数据模式 双线双向全双工
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                   //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                        //设置SPI的工作方式 主SPI
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                            //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理: 2内部NSS信号有SSI位控制
	SPI_Init(SPI2,&SPI_InitStructure);
	
	SPI_Cmd(SPI2,ENABLE);                                              //使能SPI2
	
	SPI2_ReadWriteByte(0xff);                                           //启动传输
}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
	u8 retry=0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)      //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200) return 0;
	}
	SPI_I2S_SendData(SPI2,TxData);                                 //发送一个数据
	retry=0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)    //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200) return 0;
	}
	return  SPI_I2S_ReceiveData(SPI2);                           //返回最近接受的数据	
}




