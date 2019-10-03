#include "tpad.h"
#include "delay.h"

#define TPAD_ARR_MAX_VAL 0XFFFF;         //���ARRֵ

vu16 tpad_default_val=0;                //����ʱ ����������ĳ��ʱ��

//���벶���ʼ��
void TIM5_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//��ʼ��IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;     //��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ����ʱ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=	arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	//�������벶�����  TIM5 CH2
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=0X03;                  //8����ʱ��ʱ�������˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;   //��⵽������˵��������
	TIM_ICInitStructure.TIM_ICPrescaler=0;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //TI2
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	
	TIM_Cmd(TIM5,ENABLE);                                    //ʹ�ܶ�ʱ��5
}
//����������ʼ��
//0:��ʼ���ɹ� 1:��ʼ��ʧ��
u8 TAPD_Init(u16 psc)
{
	u16 buf[10];                              //�洢10�γ��ʱ�� ����ƽ��ֵ
	u16 temp;
	u8 i,j;                                  //����ѭ�� ��������
	TIM5_Cap_Init(0XFFFF,psc-1);       //��1MHZ��Ƶ�ʼ���
	for(i=0;i<10;i++)
	{
		buf[i]=TPAD_Get_Val();
		delay_ms(10);
	}
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++) temp+=buf[i];
	tpad_default_val=temp/6;
	if(tpad_default_val>0xFFFF/2) return 1;     //�����ʼ����������TPAD_ARR_MAX_VAL/2 ˵����ʼ��ʧ��
	return 0;                                   //��ʼ���ɹ�����0
}

//��λһ��
void TPAD_Reset(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;     //�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PA.1���0,�ŵ�
	
	delay_ms(5);                                 //��ʱ5Ms
	
	TIM_SetCounter(TIM5,0);                         //����������ֵ��0
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC2);    //����жϱ�־λ
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;     //���ø������� �ȴ����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//�õ���ʱ������ֵ
//�����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	while(TIM_GetITStatus(TIM5,TIM_IT_CC2)==RESET)    // �ȴ�����������
	{
		if(TIM_GetCounter(TIM5)>0xFFFF-500) return TIM_GetCounter(TIM5);  //�����ʱ ����CNT
	}
	return TIM_GetCapture2(TIM5);	                         //���س��ʱ��
}

//��ȡn��,ȡ���ֵ
//n��������ȡ�Ĵ���
//����ֵ��n�ζ������������������ֵ
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();
		if(temp>res) res=temp;
	}
	return res;
}

//ɨ�败������
//mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
//����ֵ:0,û�а���;1,�а���;										  
#define TPAD_GATE_VAL 	100	//����������ֵ,Ҳ���Ǳ������tpad_default_val+TPAD_GATE_VAL,����Ϊ����Ч����.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;
	u8 res=0;
	u8 sample=3;                        //Ĭ�ϲ�������Ϊ3
	u16 rval;
	if(mode)
	{
		keyen=0;
		sample=6;                         //֧�������������ò�������Ϊ6��
 	}
	rval=TPAD_Get_MaxVal(sample);      //�������� ��ȡ���ֵ
	if(rval>tpad_default_val+TPAD_GATE_VAL)    //���ڳ��ʱ�� ������Ч
	{
		if(keyen==0) res=1;
		keyen=3;                              //����Ҫ��������֮�󰴼�������Ч
	}
	if(keyen) keyen--;
	return res;
}

