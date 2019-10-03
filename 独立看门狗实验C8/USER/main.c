#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "wdg.h"
 int main(void)
{
	//WDG_Init(4,625);            //设置溢出时间为1s Tout=((4*z^prer)*rlr)/40(ms)
	LED_Init();
	delay_init();
	led=0;
	while(1)
	{
		delay_ms(500);
		led=1;                        //用于观察单片机是否复位
		//delay_ms(600);
		//IWDG_ReloadCounter();
	}
	
}


