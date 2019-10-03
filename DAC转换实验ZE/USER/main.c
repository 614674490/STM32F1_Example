#include "stm32f10x.h" 
#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "adc.h"
#include "oled.h"
#include "usart.h"
#include "dac.h"
#include "iic.h"
 int main(void)
{
	u32 dir=1;
	u32 val=0;
	u16 adcx,adcx2;
	float temp,temp2;
	LED_Init();
	delay_init();
	IIC_Init();
  OLED_Init();
	Adc_Init();
	Dac1_Init();
	uart_init(115200); 
	OLED_Display();
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);//初始值为0	    
	while(1)
	{
		if(dir) val++;
		else val--;
		if(val>3300) dir=0;
		else if(val==0) dir=1;              //要这样写才能递减递加循环
		Dac1_Set_Vol(val);                    //输出ADC的值
		DACWrite(0x90,0x40,250);       //输入数值不能超过255
		//显示DAC输出的值
		adcx=DAC_GetDataOutputValue(DAC_Channel_1);
		OLED_ShowNum(32,0,adcx,4,16);             //显示直接获取的数值
		temp=(float)(adcx*(3.3/4096));            //转换成电压
		adcx=temp;                               //获取整数部分
		OLED_ShowNum(32,2,adcx,1,16);
		temp-=adcx;                         //获取小数部分
		temp=temp*1000;                          //精确到千分位
		OLED_ShowNum(48,2,temp,3,16);             //显示小数
		
		//显示ADC接受的值
		adcx2=Get_Adc_Average(ADC_Channel_1,10);
		OLED_ShowNum(32,4,adcx2,4,16);             //显示直接获取的数值
		temp2=(float)(adcx2*(3.3/4096));            //转换成电压
		adcx2=temp2;                               //获取整数部分
		OLED_ShowNum(32,6,adcx2,1,16);
		temp2-=adcx2;                         //获取小数部分
		temp2=temp2*1000;                          //精确到千分位
		OLED_ShowNum(48,6,temp2,3,16);             //显示小数
	}
	
}





