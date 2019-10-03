#include "sys.h"
#include "usart3.h"
u8 USART2_RX_BUF[USART2_REC_LEN]; //���������洢���յ������ݣ���USART3_REC_LENΪ����ܽ��յ��ֽ��޶�
u16 USART2_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
void usart2_init(u32 bound)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructurea;
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
  
	  NVIC_InitStructurea.NVIC_IRQChannel=USART2_IRQn;
	  NVIC_InitStructurea.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructurea.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructurea.NVIC_IRQChannelSubPriority=2;
	  NVIC_Init(&NVIC_InitStructurea);          //����3�жϵ�����
		
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//����3�ж�ʹ�ܺ��ж���������
		
		USART_Cmd(USART2,ENABLE);//����3ʹ��
	
	  
}
void USART2_IRQHandler()//����3���ж�
{ 
	 u8 res;//��ʱ������յ�����
   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����3�����ж�
	 {
		   
	     res=USART_ReceiveData(USART2);//���յ����ݷŽ�res
		   USART2_RX_BUF[USART2_RX_STA&0x7FFF]=res;//���ݷŽ������У�������õ�main��������
		   USART2_RX_STA++;                         //�ֽڳ���++
		 if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	+; 
		  USART2_RX_STA|=0x8000;//����3�������
		 USART_ClearITPendingBit(USART2,USART_IT_RXNE);//��������жϱ�־
		   
	  }
}



