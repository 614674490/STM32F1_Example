#include "adc.h"
#include "delay.h"


//ADC1_ch1->PA1  ch2->PA2
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA,ENABLE);  //使能IO口和ADC时钟
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                 //模拟输入引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;                   //PA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_DeInit(ADC1);                                     //复位ADC1 
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6 );                  //设置ADC分频 72MHZ/6=12 转化时间最小为1us 即14个周期
	
	//初始化ADC
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;       //单次转换
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;     //数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;    ////转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;       //独立模式
	ADC_InitStructure.ADC_NbrOfChannel=1;	                  //顺序进行规则转换的ADC通道的数目
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;                      //单通道
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);                                //使能ADC1
	
	//校准
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}

//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);                    //开启软件转换
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));            //等待转换完成

	return ADC_GetConversionValue(ADC1);                         // 读取ADC的值
}

u16 Get_Adc_Average(u8 ch,u8 times)    //通道 采样次数
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