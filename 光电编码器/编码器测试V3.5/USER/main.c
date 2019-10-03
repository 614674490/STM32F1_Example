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
	delay_init(72);	     //延时初始化
	NVIC_Configuration();
	Timerx_Init(100,7199);//10Khz的计数频率，计数到5000为500ms,100为10ms
	//LED_Init();
	Encoder_Init();
	delay_ms(100); //等待主机spi初始化完毕 非常重要！

	usart1_init(9600);//USART1_Config
	delay_ms(100);
	IWDG_Init(IWDG_Prescaler_64,625);//1秒钟的看门狗
	delay_ms(200);
	IWDG_Feed();//喂狗
    while(1)
    {
        IWDG_Feed();//喂狗
		usart1_server();
		Encoder_server();//Printf_encoder
	}
}

void usart1_server(void)
{
    //公司保密的东西就不发出来了,这里删除了其他的，只剩一个return
	return;
}
void Encoder_server(void)
{
	/*硬件连接的是PA6和PA7 必须是这两个脚，外部用的是高速光耦隔离 PC410光耦	增量型AB相编码器 */
	if(delay_time2 == 0){
		delay_time2=50;
		Printf_encoder(Printf_TIMCNT);
	}
}
