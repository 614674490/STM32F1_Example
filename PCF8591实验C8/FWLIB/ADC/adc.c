#include "adc.h"
#include "delay.h"


//ADC1_ch1->PA1  ch2->PA2
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA,ENABLE);  //ʹ��IO�ں�ADCʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                 //ģ����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;                   //PA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);                                     //��λADC1 
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6 );                  //����ADC��Ƶ 72MHZ/6=12 ת��ʱ����СΪ1us ��14������
	
	//��ʼ��ADC
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;       //����ת��
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;     //�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;    ////ת��������������ⲿ��������
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;       //����ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=1;	                  //˳����й���ת����ADCͨ������Ŀ
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;                      //��ͨ��
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);                                //ʹ��ADC1
	
	//У׼
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);                    //�������ת��
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));            //�ȴ�ת�����

	return ADC_GetConversionValue(ADC1);                         // ��ȡADC��ֵ
}

u16 Get_Adc_Average(u8 ch,u8 times)    //ͨ�� ��������
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	