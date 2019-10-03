#include "usart3.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  

//���ڷ��ͻ����� 	
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
                                                 //__align()�����޸���߼��������ֽڱ߽� ����֤���ݶ�������Ӧ����
#ifdef USART3_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART3_RX_STA=0;   	 
void USART3_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)         //���յ�����
	{
		res=USART_ReceiveData(USART3);
		if(USART3_RX_STA<USART3_MAX_RECV_LEN)                   //���ܽ�������
		{
			TIM_SetCounter(TIM3,0);                                //��ռ�����
			if(USART3_RX_STA==0) TIM3_Set(1);                    //ʹ�ܽ����ж�
			USART3_RX_BUF[USART3_RX_STA]=res;                      //��¼���յ���ֵ
			USART3_RX_STA++;
		}
		else
			USART3_RX_STA|=1<<15;                               //��������ֽ� ǿ�ƽ������
	}
}
//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART3_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	USART_DeInit(USART3);                                //��λ����3
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;           //PB10 TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;           //PB11 RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=bound;                 //һ��Ϊ9600
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; //�ֳ�Ϊ8λ���ݳ���
	USART_InitStructure.USART_StopBits=USART_StopBits_1;      //һ��ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;         //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;               //�շ�ģʽ
	USART_Init(USART3,&USART_InitStructure);
	
	USART_Cmd(USART3,ENABLE);                                   //ʹ�ܴ���3
#ifdef USART3_RX_EN		  	//���ʹ���˽���

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�  
	
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;         //��Ӧ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM3_Init(99,7199);		//50ms�ж�
	USART3_RX_STA=0;		//����
	TIM3_Set(0);			//�رն�ʱ��3
#endif
}

//��ʱ��3�жϷ������		    
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)    //���������ж�
	{
		USART3_RX_STA|=1<<15;                             //��ǽ������
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);       //����жϱ�־λ
		TIM3_Set(0);                                      //�رն�ʱ��3
	}
}
//����TIM3�Ŀ���
//sta:0���ر�;1,����;
void TIM3_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM3,0);//���������
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	
	}else TIM_Cmd(TIM3, DISABLE);//�رն�ʱ��4	   
}

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM3_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��//TIM3ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
#endif		 
