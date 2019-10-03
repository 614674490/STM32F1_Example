#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //引用延时函数头文件
#include "oled.h"
#include "adc.h"
#include "timer.h"
int main()
{
	u32 dir=1;
	u32 val=0;
	u16 adcx,adcx2;
	float temp,temp2;
	delay_init();
	OLED_GPIO_Init();
	OLED_Init();
	Adc_Init();
	TIM1_PWM_Init(255,0);       //PWM初始化 255+1=2^8 分辨率为8 分频系数为0
	TIM_SetCompare1(TIM1,val);
	OLED_Display();
	while(1)
	{
		if(dir) val++;
		else val--;
		if(val>=255) dir=0;
		else if(val==0) dir=1;               //这句话不能写成 else dir=1
		TIM_SetCompare1(TIM1,val);
		//显示DAC输出的值
		adcx=TIM_GetCapture1(TIM1);
		OLED_ShowNum(32,0,adcx,4,16);             //显示直接获取的数值
		temp=(float)(adcx*(3.3/256));            //转换成电压
		adcx=temp;                               //获取整数部分
		OLED_ShowNum(32,2,adcx,1,16);
		temp-=adcx;                         //获取小数部分
		temp=temp*1000;                          //精确到千分位
		OLED_ShowNum(48,2,temp,3,16);             //显示小数
		
		//显示ADC接受的值
		adcx2=Get_Adc_Average(ADC_Channel_1,10);
		OLED_ShowNum(32,4,adcx2,4,16);             //显示直接获取的数值
		temp2=(float)(adcx2*(3.3/256));            //转换成电压
		adcx2=temp2;                               //获取整数部分
		OLED_ShowNum(32,6,adcx2,1,16);
		temp2-=adcx2;                         //获取小数部分
		temp2=temp2*1000;                          //精确到千分位
		OLED_ShowNum(48,6,temp2,3,16);             //显示小数
	}
}


