#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "wdg.h"
 int main(void)
{
	//WDG_Init(4,625);            //�������ʱ��Ϊ1s Tout=((4*z^prer)*rlr)/40(ms)
	LED_Init();
	delay_init();
	led=0;
	while(1)
	{
		delay_ms(500);
		led=1;                        //���ڹ۲쵥Ƭ���Ƿ�λ
		//delay_ms(600);
		//IWDG_ReloadCounter();
	}
	
}


