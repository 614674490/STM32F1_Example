#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"

void My_USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);   //���ں�GPIO��ʹ��
	
	//GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;    //PA9 TXD �������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    //PA10 RXD ��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//���ڳ�ʼ��
	USART_InitStructure.USART_BaudRate=115200;               //���ò�����
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;    //������Ӳ����
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                      //ʹ�ܷ��ͺͽ���ģʽ
	USART_InitStructure.USART_Parity=USART_Parity_No;                                //��ʹ����żУ��λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                             //����ֹͣλ 1
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                        //�����ֳ� 8λ
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);                                                        //ʹ�ܴ���1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);                                     //ʹ�ܽ����ж�
	
	//�����жϷ����ʼ��
	NVIC_InitInitStructure.NVIC_IRQChannel=USART1_IRQn;                              //����1ͨ��
	NVIC_InitInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitInitStructure.NVIC_IRQChannelPreemptionPriority=1;                      //��ռ���ȼ�Ϊ1
	NVIC_InitInitStructure.NVIC_IRQChannelSubPriority=1;                             //��Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitInitStructure);
}

//��д�жϴ�����
void USART1_IRQHandler(void)
{
	u16 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))                                   //�ж��Ƿ��������ж�
	{
		res=USART_ReceiveData(USART1);                                                 //��������
		if(res==0x01)                                             //�ڴ���������ҩѡ��16���Ʒ��ͣ�������յ������ı���ʽ������ȷ��16����������Ҫ�رշ�������
			{
				led=0;
			}
			else if(res==0x00)
			{
				led=1;
			}
	  USART_SendData(USART1,res);                                                    //��������
	}
}
	
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //����Ϊ��2��  2λ��ռ 2λ��Ӧ
	My_USART1_Init();
	LED_Init();
	led=1;
	while(1)
	{
		//led=0;
	}
}


