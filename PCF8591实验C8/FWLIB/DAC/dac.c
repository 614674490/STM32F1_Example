#include "dac.h"

//DAC通道1输出初始化
void Dac1_Init(void)
{
	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA时钟
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC,ENABLE);    //使能DAC时钟
	
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;    //屏蔽、幅形设置
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;               //DAC输出缓存关闭
	DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;        //不使用触发功能
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;              //不使用波形发生
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);            //根据参数初始化DAC通道1
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;        //模拟输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;            //PA4->DAC1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);                  //PA.4 输出高
	
	DAC_Cmd(DAC_Channel_1,ENABLE);                          //使能DAC转换通道
	
	DAC_SetChannel1Data(DAC_Align_12b_R,0);        //12为右对齐 初始化DAC输出值
}

//DAC值与电压值转换
//0-3300=0-3.3
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp=temp/1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp); 
}
