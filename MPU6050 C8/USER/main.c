#include "stm32f10x.h" 
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
 int main(void)
{
	float temp;
	float pitch,roll,yaw; 		//欧拉角
	delay_init();
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	while(MPU_Init());
	while(mpu_dmp_init())
	{
		led = !led;
	}		
	while(1)
	{
		delay_ms(200);
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			temp=pitch*10;
			printf("俯仰角:%.3f  ",temp);
			temp=roll*10;
			printf("横滚角:%.3f  ",temp);
			temp=yaw*10;
			printf("航向角:%.3f  \r\n\r\n",temp);
		}
	}
}


