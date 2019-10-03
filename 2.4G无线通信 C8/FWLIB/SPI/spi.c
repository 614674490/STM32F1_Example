#include "spi.h"


void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);       //ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               //�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //SCLK MISO MOSI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256,��ԽС�������ٶȾ�Խ��
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                         //����ͬ��ʱ�ӵڶ��������ؽ������ݲ���
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                          //����ͬ��ʱ�ӿ���״̬λ�ߵ�ƽ
	SPI_InitStructure.SPI_CRCPolynomial=7;                             //CRCֵ����Ķ���ʽ ����У׼
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                    //ѡ�����ݴ�С SPI���ͽ���8λ�ֽ�
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;   //ѡ�������˫�������ģʽ ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                   //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                        //����SPI�Ĺ�����ʽ ��SPI
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                            //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������: 2�ڲ�NSS�ź���SSIλ����
	SPI_Init(SPI2,&SPI_InitStructure);
	
	SPI_Cmd(SPI2,ENABLE);                                              //ʹ��SPI2
	
	SPI2_ReadWriteByte(0xff);                                           //��������
}

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{
	u8 retry=0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)      //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200) return 0;
	}
	SPI_I2S_SendData(SPI2,TxData);                                 //����һ������
	retry=0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)    //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200) return 0;
	}
	return  SPI_I2S_ReceiveData(SPI2);                           //����������ܵ�����	
}




