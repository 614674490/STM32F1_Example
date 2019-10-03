#include "adc.h"
#include "delay.h"

//��ʼ��ADC
//����ADC1��ͨ��0~7,��Ӧ��ϵ����
//ADC1_CH0-->PA0	
//ADC1_CH1-->PA1	
//ADC1_CH2-->PA2
//ADC1_CH3-->PA3	
//ADC1_CH4-->PA4	
//ADC1_CH5-->PA5	
//ADC1_CH6-->PA6
//ADC1_CH7-->PA7
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA,ENABLE);  //ʹ��IO�ں�ADCʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                 //ģ����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;    //PA0-1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);                                     //��λADC1 
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6 );                  //����ADC��Ƶ 72MHZ/6=12 ת��ʱ����СΪ1us ��14������
	
	//��ʼ��ADC
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;       //ѭ��ģʽ
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;     //�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;    ////ת��������������ⲿ��������
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;       //����ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=2;	                  //˳����й���ת����ADCͨ������Ŀ
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;            //ɨ��ģʽ
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);                                //ʹ��ADC1
	ADC_DMACmd(ADC1,ENABLE);                          //ʹ��ADC��DMAͨ��
	
	//У׼
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}


