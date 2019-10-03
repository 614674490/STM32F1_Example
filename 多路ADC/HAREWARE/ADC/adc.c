#include "adc.h"
#include "delay.h"

//初始化ADC
//开启ADC1的通道0~7,对应关系如下
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA,ENABLE);  //使能IO口和ADC时钟
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                 //模拟输入引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;    //PA0-1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);                                     //复位ADC1 
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6 );                  //设置ADC分频 72MHZ/6=12 转化时间最小为1us 即14个周期
	
	//初始化ADC
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;       //循环模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;     //数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;    ////转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;       //独立模式
	ADC_InitStructure.ADC_NbrOfChannel=2;	                  //顺序进行规则转换的ADC通道的数目
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;            //扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);                                //使能ADC1
	ADC_DMACmd(ADC1,ENABLE);                          //使能ADC的DMA通道
	
	//校准
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}


