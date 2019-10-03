#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //������ʱ����ͷ�ļ�
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
	TIM1_PWM_Init(255,0);       //PWM��ʼ�� 255+1=2^8 �ֱ���Ϊ8 ��Ƶϵ��Ϊ0
	TIM_SetCompare1(TIM1,val);
	OLED_Display();
	while(1)
	{
		if(dir) val++;
		else val--;
		if(val>=255) dir=0;
		else if(val==0) dir=1;               //��仰����д�� else dir=1
		TIM_SetCompare1(TIM1,val);
		//��ʾDAC�����ֵ
		adcx=TIM_GetCapture1(TIM1);
		OLED_ShowNum(32,0,adcx,4,16);             //��ʾֱ�ӻ�ȡ����ֵ
		temp=(float)(adcx*(3.3/256));            //ת���ɵ�ѹ
		adcx=temp;                               //��ȡ��������
		OLED_ShowNum(32,2,adcx,1,16);
		temp-=adcx;                         //��ȡС������
		temp=temp*1000;                          //��ȷ��ǧ��λ
		OLED_ShowNum(48,2,temp,3,16);             //��ʾС��
		
		//��ʾADC���ܵ�ֵ
		adcx2=Get_Adc_Average(ADC_Channel_1,10);
		OLED_ShowNum(32,4,adcx2,4,16);             //��ʾֱ�ӻ�ȡ����ֵ
		temp2=(float)(adcx2*(3.3/256));            //ת���ɵ�ѹ
		adcx2=temp2;                               //��ȡ��������
		OLED_ShowNum(32,6,adcx2,1,16);
		temp2-=adcx2;                         //��ȡС������
		temp2=temp2*1000;                          //��ȷ��ǧ��λ
		OLED_ShowNum(48,6,temp2,3,16);             //��ʾС��
	}
}


