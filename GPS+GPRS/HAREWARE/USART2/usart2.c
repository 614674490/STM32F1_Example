#include "usart2.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  

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
void USART2_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)         //���յ�����
	{
		res=USART_ReceiveData(USART2);
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)                   //���ܽ�������
		{
			TIM_SetCounter(TIM2,0);                                //��ռ�����
			if(USART2_RX_STA==0) TIM2_Set(1);                    //ʹ�ܽ����ж�
			USART2_RX_BUF[USART2_RX_STA]=res;                      //��¼���յ���ֵ
			USART2_RX_STA++;
		}
		else
			USART2_RX_STA|=1<<15;                               //��������ֽ� ǿ�ƽ������
	}
}
//����һ���ֽ�
void USART2_SendByte(u8 Data)
{
   //�ȴ��������ݼĴ�����
   while (!(USART2->SR & USART_FLAG_TXE));
   //��������
   USART2->DR = (Data & 0x00FF);
   //�ȴ��������
   while (!(USART2->SR & USART_FLAG_TC));   
}
//���Ͷ���ֽ�����
void USART2_Write(u8* data,u16 len)
{
	u16 i;

	for (i=0; i<len; i++)
	  USART2_SendByte(data[i]);	

}
//����ATָ��
void SendAT(u8 *dat,u16 num)
{    
    USART2_Write(dat,num);     
    USART2_SendByte(0x0D);  
	USART2_SendByte(0x0A);        
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ATָ��
void SendATData(u8 *dat)
{
    u16 num;
	  num=strlen((char*)dat);
    USART2_Write(dat,num);     
    USART2_SendByte(0x1A); 
       
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
	
	USART_Cmd(USART2,ENABLE);                                   //ʹ�ܴ���2
#ifdef USART2_RX_EN		  	//���ʹ���˽���

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�  
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;         //��Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM2_Init(99,7199);		//50ms�ж�
	USART2_RX_STA=0;		//����
	TIM2_Set(0);			//�رն�ʱ��2
#endif
}

//��ʱ��2�жϷ������		    
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)    //���������ж�
	{
		USART2_RX_STA|=1<<15;                             //��ǽ������
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);       //����жϱ�־λ
		TIM2_Set(0);                                      //�رն�ʱ��2
	}
}
//����TIM2�Ŀ���
//sta:0���ر�;1,����;
void TIM2_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM2,0);//���������
		TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx	
	}else TIM_Cmd(TIM2, DISABLE);//�رն�ʱ��4	   
}

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM2_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��//TIM2ʱ��ʹ��    
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
#endif		 

