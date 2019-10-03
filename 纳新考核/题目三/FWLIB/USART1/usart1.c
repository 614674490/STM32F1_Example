#include "usart1.h"
#include "stm32f10x_usart.h"
#include "led.h"
#include "stm32f10x_tim.h"
u8 USART1_RX_BUF[USART1_REC_LEN]; //���������洢���յ������ݣ���USART3_REC_LENΪ����ܽ��յ��ֽ��޶�
u16 USART1_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
//��ʱ��3�жϷ������		    
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)    //���������ж�
	{
		USART1_RX_STA|=1<<15;                             //��ǽ������
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
void My_USART1_Init(void)
{
	//GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//??USART1,GPIOA??
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.9
   
  //USART1_RX	  GPIOA.10???
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.10  

  //Usart1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
  
   //USART ?????

	USART_InitStructure.USART_BaudRate =115200;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

  USART_Init(USART1, &USART_InitStructure); //?????1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//????????
  USART_Cmd(USART1, ENABLE);                    //????1 
	//TIM3_Init(99,7199);		//10ms�ж�
	USART1_RX_STA=0;		//����
	//TIM3_Set(0);			//�رն�ʱ��3

}
//��д�жϴ�����
/*void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)         //���յ�����
	{
		res=USART_ReceiveData(USART1);
		if(USART1_RX_STA<USART1_REC_LEN)                   //���ܽ�������
		{
			TIM_SetCounter(TIM3,0);                                //��ռ�����
			if(USART1_RX_STA==0) TIM3_Set(1);                    //ʹ�ܽ����ж�
			USART1_RX_BUF[USART1_RX_STA&0x7fff]=res;                      //��¼���յ���ֵ
			USART1_RX_STA++;
		}
		else
			USART1_RX_STA|=1<<15;                               //��������ֽ� ǿ�ƽ������
	}
}*/
u16 len;
u8 flag=0;
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)         //���յ�����
	{
		res=USART_ReceiveData(USART1);
		if(!flag)
		{
			USART1_RX_BUF[USART1_RX_STA&0x7fff]=res;                      //��¼���յ���ֵ
			len=(++USART1_RX_STA)&0x7fff;
		}
		if(USART1_RX_BUF[0]!=0xaa)
			USART1_RX_STA=0;
		else
		{
			if(USART1_RX_BUF[len-1]==0x55)
			{
				USART1_RX_STA|=0x8000;
				flag=1;
			}
		}
		if((USART1_RX_STA&0x7fff)>USART1_REC_LEN)
			USART1_RX_STA|=0x8000;                        //ǿ�ƽ������
		USART_ClearITPendingBit(USART1,USART_IT_RXNE );
	}
}
			
		

