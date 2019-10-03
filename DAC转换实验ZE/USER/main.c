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
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);//��ʼֵΪ0	    
	while(1)
	{
		if(dir) val++;
		else val--;
		if(val>3300) dir=0;
		else if(val==0) dir=1;              //Ҫ����д���ܵݼ��ݼ�ѭ��
		Dac1_Set_Vol(val);                    //���ADC��ֵ
		DACWrite(0x90,0x40,250);       //������ֵ���ܳ���255
		//��ʾDAC�����ֵ
		adcx=DAC_GetDataOutputValue(DAC_Channel_1);
		OLED_ShowNum(32,0,adcx,4,16);             //��ʾֱ�ӻ�ȡ����ֵ
		temp=(float)(adcx*(3.3/4096));            //ת���ɵ�ѹ
		adcx=temp;                               //��ȡ��������
		OLED_ShowNum(32,2,adcx,1,16);
		temp-=adcx;                         //��ȡС������
		temp=temp*1000;                          //��ȷ��ǧ��λ
		OLED_ShowNum(48,2,temp,3,16);             //��ʾС��
		
		//��ʾADC���ܵ�ֵ
		adcx2=Get_Adc_Average(ADC_Channel_1,10);
		OLED_ShowNum(32,4,adcx2,4,16);             //��ʾֱ�ӻ�ȡ����ֵ
		temp2=(float)(adcx2*(3.3/4096));            //ת���ɵ�ѹ
		adcx2=temp2;                               //��ȡ��������
		OLED_ShowNum(32,6,adcx2,1,16);
		temp2-=adcx2;                         //��ȡС������
		temp2=temp2*1000;                          //��ȷ��ǧ��λ
		OLED_ShowNum(48,6,temp2,3,16);             //��ʾС��
	}
	
}





