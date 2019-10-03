#include "usart2.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "stm32f10x_dma.h"
//���ڷ��ͻ����� 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
                                                 //__align()�����޸���߼��������ֽڱ߽� ����֤���ݶ�������Ӧ����
#ifdef USART2_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART2_RX_STA=0;   	 
u16 len;
u8 flag=0;
u8 rx_buf[USART2_MAX_RECV_LEN];
void USART2_IRQHandler(void)
{
	u8 res;
	u8 i;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)         //���յ�����
	{
		res=USART_ReceiveData(USART2);
		if(!flag)
		{
			USART2_RX_BUF[USART2_RX_STA&0x7fff]=res;                      //��¼���յ���ֵ
			len=(++USART2_RX_STA)&0x7fff;
		}
		if(USART2_RX_BUF[0]!=0xaa)
			USART2_RX_STA=0;
		else
		{
			if(USART2_RX_BUF[len-1]==0x55)
			{
				for(i=1;i<len-1;i++)
				{
					rx_buf[i-1]=USART2_RX_BUF[i];
				}
				USART2_RX_STA|=0x8000;
				flag=1;
			}
		}
		if((USART2_RX_STA&0x7fff)>USART2_MAX_RECV_LEN)
			USART2_RX_STA|=0x8000;                        //ǿ�ƽ������
		USART_ClearITPendingBit(USART2,USART_IT_RXNE );
	}
}
			

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART2_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_DeInit(USART2);                                //��λ����2
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;           //PA2 TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;           //PA3 RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=bound;                 //һ��Ϊ9600
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; //�ֳ�Ϊ8λ���ݳ���
	USART_InitStructure.USART_StopBits=USART_StopBits_1;      //һ��ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;         //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;               //�շ�ģʽ
	USART_Init(USART2,&USART_InitStructure);
	
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);                     //ʹ�ܴ���2��DMA����
	UART_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF);//DMA1ͨ��7,����Ϊ����2,�洢��ΪUSART2_TX_BUF 
	USART_Cmd(USART2,ENABLE);                                   //ʹ�ܴ���2
#ifdef USART2_RX_EN		  	//���ʹ���˽���

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�  
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;         //��Ӧ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART2_RX_STA=0;		//����
#endif
}

//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA_GetCurrDataCounter(DMA1_Channel7)!=0);	//�ȴ�ͨ��7�������   
	UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART2_TX_BUF)); 	//ͨ��dma���ͳ�ȥ
}


#endif		 

///////////////////////////////////////USART2 DMA�������ò���//////////////////////////////////	   		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ    
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
{
	DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
  DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	
} 

//����һ��DMA����
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
{
	DMA_Cmd(DMA_CHx,DISABLE);             //ʧ��DMA
	DMA_SetCurrDataCounter(DMA_CHx,len);   // DMAͨ����DMA����Ĵ�С	
  DMA_Cmd(DMA_CHx,ENABLE);             //ʹ��DMA
}




