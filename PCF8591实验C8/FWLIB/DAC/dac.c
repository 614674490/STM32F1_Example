#include "dac.h"

//DACͨ��1�����ʼ��
void Dac1_Init(void)
{
	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOAʱ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC,ENABLE);    //ʹ��DACʱ��
	
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;    //���Ρ���������
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;               //DAC�������ر�
	DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;        //��ʹ�ô�������
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;              //��ʹ�ò��η���
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);            //���ݲ�����ʼ��DACͨ��1
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;        //ģ������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;            //PA4->DAC1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);                  //PA.4 �����
	
	DAC_Cmd(DAC_Channel_1,ENABLE);                          //ʹ��DACת��ͨ��
	
	DAC_SetChannel1Data(DAC_Align_12b_R,0);        //12Ϊ�Ҷ��� ��ʼ��DAC���ֵ
}

//DACֵ���ѹֵת��
//0-3300=0-3.3
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp=temp/1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp); 
}
