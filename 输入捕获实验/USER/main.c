#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "led.h"
extern u8  TIM2CH2_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM2CH2_CAPTURE_VAL;	//输入捕获值

int main(void)
{
	u32 temp=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	LED_Init();
	TIM2_Cap_Init(0xFFFF,72-1);   //以1MHZ计数 一个时钟周期为1us
	uart_init(115200);
	relay=0;
	while(1)
	{
		if(TIM2CH2_CAPTURE_STA&0x80)        //成功捕获一次上升沿
		{
			temp=TIM2CH2_CAPTURE_STA&0x3F;    //记录溢出次数
			temp=temp*65536;                  //每一处一次就是65536us
			temp+=TIM2CH2_CAPTURE_VAL;        //加上剩余高电平时长
			printf("HIGH:%d us\r\n",temp);    //输出高电平时长
			TIM2CH2_CAPTURE_STA=0;        //重新捕获上升沿
		}
	}		
}









