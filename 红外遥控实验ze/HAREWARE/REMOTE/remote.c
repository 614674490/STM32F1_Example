#include "remote.h"
#include "delay.h"
#include "led.h"
//����ң�س�ʼ�� PWM�����ȵ���
//����IO�Լ���ʱ��2�����벶��
void Remote_Init(void)    		
{
	//TIM2_CH2->PA1
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	//��ʼ��IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;         //PA1 ��������  ���������ٶ�
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;             //PA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);						       //PA1 ����
	
	//��ʼ����ʱ��2 ������Ƶ��Ϊ1Mhz 10ms���һ��
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //����ʱ�ӷָ� TDTS=0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ü���ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period=10000;                      //�Զ�����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=71;                   //Ԥ��Ƶ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//��ʼ�����벶��ͨ��
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;             //CC1S=01 ѡ������� IC2ӳ�䵽TI2�� ͨ��2
	TIM_ICInitStructure.TIM_ICFilter=0x03;                     //���������˲��� 8����ʱ��ʱ�������˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //���ò���ʽ �����ز���
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;        //���������Ƶ ����Ƶ
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI ;  //ѡ��ӳ�� ӳ�䵽TI2�� ͨ��2Ҫӳ�䵽ͨ��1ѡ���ӣ���ֱ֮��
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	//�жϳ�ʼ������
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);      //��������жϺ�CC2E�����ж�
	
	TIM_Cmd(TIM2,ENABLE);                                   //ʹ�ܶ�ʱ��2
}

//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����	
//[4]:����������Ƿ��Ѿ�������								   
//[3:0]:�����ʱ��
u8 	RmtSta=0;	  	  
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec=0;	//������յ�������	   		    
u8  RmtCnt=0;	//�������µĴ���	  
//��ʱ��4�жϷ������	 
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)          //���������ж�
	{
		if(RmtSta&0x80)                                       //�ϴ������ݽ��յ�
		{
			RmtSta&=~0x10;                                      //ȡ�������ز����־
			if((RmtSta&0x0F)==0x00)  RmtSta|=1<<6;               //����Ѿ����һ�ΰ�����Ϣ�Ĳɼ�
			if((RmtSta&0x0F)<14)      RmtSta++;
			else
			{
				RmtSta&=~(1<<7);                                  //���������־
				RmtSta&=0xF0;                                     //����������
			}
		}
	}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)          //���������ж�
	{
		if(RDATA)                                             //�����ز���
		{
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);   //����Ϊ�½��ز���
			TIM_SetCounter(TIM2,0);                                 //��ռ�����
			RmtSta|=0x10;                                           //����������Ѿ�����
		}
		else                                                      //�½��ز���
		{
			Dval=TIM_GetCapture2(TIM2);                            //��ȡ��������ֵ
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);   //����Ϊ�����ز���
			if(RmtSta&0x10)                                       //����������
			{
				if(RmtSta&0x80)                                          //���յ���������
				{
					if(Dval>300&&Dval<800)         //�ߵ�ƽΪ560us
					{
						RmtRec<<=1;                   //����һλ
						RmtRec|=0;                   //�߼�0
					}
					else if(Dval>1400&&Dval<1800)    //�ߵ�ƽΪ1680us
					{
						RmtRec<<=1;                   //����һλ
						RmtRec|=1;                   //�߼�1
					}
					else if(Dval>2200&&Dval<2600)        //2500us ��������ֵ����Ϣ
					{
						RmtCnt++;                           //���������Լ�
						RmtSta&=0xF0;                       //��ռ�����
					}
				}
				else if(Dval>4200&&Dval<4700)   //���ڽ���������  4500ms
				{
					RmtSta|=1<<7;                 //��ǽ��յ������룻
					RmtCnt=0;                     //��հ�������
				}
			}
			RmtSta&=~(1<<4);
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC2);         //����жϱ�־λ
}

//����������
//����ֵ:
//	 0,û���κΰ�������
//����,���µİ�����ֵ.
u8 Remote_Scan(void)
{
	u8 sta=0;
	u8 t1,t2;
	if(RmtSta&(1<<6))    //�õ���һ��������������Ϣ
	{
		t1=RmtRec>>24;      //�õ���ַ��
		t2=(RmtRec>>16)&0xff;   //�õ���ַ����
		if((t1==(u8)~t2)&&(t1==REMOTE_ID))       //���ң��ID����ַ��
		{
			t1=RmtRec>>8;                              //������У��
			t2=RmtRec;
			if(t1==(u8)~t2) sta=t1;    //��ֵ��ȷ
		}
		if((sta==0)||(RmtSta&0x80)==0)   //�������ݴ��� ң��δ������
		{
			RmtSta&=~(1<<6);                //������յ���Ч�����ı�־
			RmtCnt=0;                       //��հ�������
		}
	}
	return sta;
}




