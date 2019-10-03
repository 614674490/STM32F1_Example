#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "remote.h"
#include "oled.h"
int main()
{
	u8 key;
	u8 *str=0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_Init();
	OLED_GPIO_Init();
	OLED_Init();
	OLED_Display();
	Remote_Init();
	while(1)
	{
		key=Remote_Scan();
		if(key)
		{
			OLED_ShowNum(56,2,key,3,16);     //显示键值
			OLED_ShowNum(56,4,RmtCnt,3,16);//显示按键次数
			switch(key)
			{
				case 0:str="ERROR";break;			   
				case 162:str="switch";break;	    
				case 98:str="mode  ";break;	    
				case 226:str="quiet ";break;		 
				case 34:str="start ";break;		  
				case 2:str="<     ";break;	   
				case 194:str=">     ";break;		  
				case 224:str="eq    ";break;		  
				case 168:str="vol-  ";break;		   
				case 144:str="vol+  ";break;		    
				case 152:str="rpt   ";break;		  
				case 176:str="u/sd  ";break;	   
				case 104:str="0     ";break;	    
				case 48:str="1     ";break;		    
				case 24:str="2     ";break;		    
				case 122:str="3     ";break;		  
				case 16:str="4     ";break;			   					
				case 56:str="5     ";break;	 
				case 90:str="6     ";break;
				case 66:str="7     ";break;
				case 74:str="8     ";break;
        case 82:str="9     ";break;
			}
			OLED_P8x16Str(56,6,str);     //显示含义
		}
		delay_ms(200);
		LED1=!LED1;
	}
}


