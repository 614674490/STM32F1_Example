#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //引用延时函数头文件
#include "oled.h"
#include "dma.h"
#include "usart.h"
#include "adc.h"

#define N 10 //取10次平均值
#define M 2 //2路AD

u16 value[N][M];   //存储ADC转换后的M*N个数字量的数据  N和M的位置不能交换！！！
u16 aftervalue[M];   //存储M个通道的ADC值
float avalue[M];     //存储滤波后的M个通道对应的电压值
u8 i=0,j=0;

//滤波函数
void filter(void)
{
	int sum=0;
	u8 count;
	for(i=0;i<M;i++)
	{
		for(count=0;count<N;count++)
		{
			sum+=value[count][i];
		}
		aftervalue[i]=sum/N;
		sum=0;
	}
}
int main()
{
	LED_Init();
	delay_init();
	OLED_GPIO_Init();
	OLED_Init();
	ADC1_Init();
	OLED_Display();
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&value,M*N);  //DMA1通道1 ADC1地址 数组首地址 发送数据长度
	                             //加上&
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件打开ADC转换
	DMA_Cmd(DMA1_Channel1, ENABLE);	//打开DMA功能
	while(1)
	{
		filter();                      //滤波
		OLED_ShowNum(32,0,aftervalue[0],4,16);
		OLED_ShowNum(32,4,aftervalue[1],4,16);
		//获取ADC对应电压值的整数部分
		for(i=0;i<M;i++)
		{
			avalue[i]=(float)(aftervalue[i]*(3.3/4096));
			aftervalue[i]=avalue[i];
		}
		//显示整数部分
		OLED_ShowNum(32,2,aftervalue[0],1,16);
		OLED_ShowNum(32,6,aftervalue[1],1,16);
		//获取小数部分
		for(i=0;i<M;i++)
		{
			avalue[i]-=aftervalue[i];
			avalue[i]*=100;          //保留两位小数
		}
		//显示小数部分
		OLED_ShowNum(48,2,avalue[0],2,16);
		OLED_ShowNum(48,6,avalue[1],2,16);
		delay_ms(200);
		LED0=!LED0;
	}	
}




			
	
			
		
	
	
	



