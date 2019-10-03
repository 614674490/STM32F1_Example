#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "delay.h"     //������ʱ����ͷ�ļ�
#include "oled.h"
#include "dma.h"
#include "usart.h"
#include "adc.h"

#define N 10 //ȡ10��ƽ��ֵ
#define M 2 //2·AD

u16 value[N][M];   //�洢ADCת�����M*N��������������  N��M��λ�ò��ܽ���������
u16 aftervalue[M];   //�洢M��ͨ����ADCֵ
float avalue[M];     //�洢�˲����M��ͨ����Ӧ�ĵ�ѹֵ
u8 i=0,j=0;

//�˲�����
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
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&value,M*N);  //DMA1ͨ��1 ADC1��ַ �����׵�ַ �������ݳ���
	                             //����&
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�����ADCת��
	DMA_Cmd(DMA1_Channel1, ENABLE);	//��DMA����
	while(1)
	{
		filter();                      //�˲�
		OLED_ShowNum(32,0,aftervalue[0],4,16);
		OLED_ShowNum(32,4,aftervalue[1],4,16);
		//��ȡADC��Ӧ��ѹֵ����������
		for(i=0;i<M;i++)
		{
			avalue[i]=(float)(aftervalue[i]*(3.3/4096));
			aftervalue[i]=avalue[i];
		}
		//��ʾ��������
		OLED_ShowNum(32,2,aftervalue[0],1,16);
		OLED_ShowNum(32,6,aftervalue[1],1,16);
		//��ȡС������
		for(i=0;i<M;i++)
		{
			avalue[i]-=aftervalue[i];
			avalue[i]*=100;          //������λС��
		}
		//��ʾС������
		OLED_ShowNum(48,2,avalue[0],2,16);
		OLED_ShowNum(48,6,avalue[1],2,16);
		delay_ms(200);
		LED0=!LED0;
	}	
}




			
	
			
		
	
	
	



