#include <stdio.h>
#include <string.h>
//#include <math.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_encoder.h"
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
//#include "myspi.h"
#include "exti.h"
#include "wdg.h"
#include "rtc.h"
#include "myiic.h"
#include "24cxx.h"
//#define ALL_INPUT    9
//#define ALL_OUTPUT   5 
////--------------------------------------------------------------------------------

void usart1_server(void);
//void usart2_server(void);
//void usart3_server(void);
void Encoder_server(void);
//--------------------------------------------------------
int main(void)
{
	SystemInit();
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();
	Timerx_Init(100,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms,100Ϊ10ms
	//LED_Init();
	Encoder_Init();
	delay_ms(100); //�ȴ�����spi��ʼ����� �ǳ���Ҫ��

	usart1_init(9600);//USART1_Config
	delay_ms(100);
	IWDG_Init(IWDG_Prescaler_64,625);//1���ӵĿ��Ź�
	delay_ms(200);
	IWDG_Feed();//ι��
    while(1)
    {
        IWDG_Feed();//ι��
		usart1_server();
		Encoder_server();//Printf_encoder
	}
}

void usart1_server(void)
{
    //��˾���ܵĶ����Ͳ���������,����ɾ���������ģ�ֻʣһ��return
	return;
}
void Encoder_server(void)
{
	/*Ӳ�����ӵ���PA6��PA7 �������������ţ��ⲿ�õ��Ǹ��ٹ������ PC410����	������AB������� */
	if(delay_time2 == 0){
		delay_time2=50;
		Printf_encoder(Printf_TIMCNT);
	}
}
