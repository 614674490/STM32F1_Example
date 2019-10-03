#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"      
#include "malloc.h"
#include "sdio_sdcard.h"   
 
 
  
/************************************************
 ALIENTEK战舰STM32开发板实验38
 SD卡SDIO 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//卡相对地址
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//显示块大小
}  
 int main(void)
 {	 
	u8 key;		 
	u32 sd_size;
	u8 t=0;	
	u8 *buf1=0;
	u8 *buf2=0; 
  u8 *buf3=0;	 

	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
 	my_mem_init(SRAMIN);		//初始化内部内存池
 	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"SD CARD TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/20"); 
	LCD_ShowString(30,130,200,16,16,"KEY0:Read Sector 0");	   
 	while(SD_Init())//检测不到SD卡
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	
 	POINT_COLOR=BLUE;	//设置字体为蓝色 
	show_sdcard_info();	//打印SD卡相关信息
	//检测SD卡成功 											    
	LCD_ShowString(30,150,200,16,16,"SD Card OK    ");
	LCD_ShowString(30,170,200,16,16,"SD Card Size:     MB");
	LCD_ShowNum(30+13*8,170,SDCardInfo.CardCapacity>>20,5,16);//显示SD卡容量
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0按下了
		{
			buf1=mymalloc(0,512);		//申请内存
			if(buf1==0)
			{
				printf("failed\r\n");
				continue;
			}
			if(SD_ReadDisk(buf1,0,2)==0)	//读取0扇区的内容
			{	
				LCD_ShowString(30,190,200,16,16,"USART1 Sending Data...");
				printf("SECTOR 0 DATA:\r\n");
				for(sd_size=0;sd_size<512*2;sd_size++)  //一个扇区512个字节
				{
					if(sd_size%512==0) printf("\r\n");
					printf("%x ",buf1[sd_size]);//打印0扇区数据 
				}					
				printf("\r\nDATA ENDED\r\n");
				LCD_ShowString(30,190,200,16,16,"USART1 Send Data Over!");
			}
			myfree(0,buf1);//释放内存
		 }
		if(key==KEY1_PRES)               
			{
				buf3=mymalloc(0,512);		//申请内存
				if(buf3==0)
				{
					printf("failed\r\n");
					continue;
				}
				sprintf((char *)buf3,"ALIENTEK");
				if(SD_WriteDisk((u8 *)buf3,3,1)==0)
				{
					myfree(0,buf3);//释放内存	
					LED1=!LED1;
					buf2=mymalloc(0,512);		//申请内存
					if(buf2==0)
					{
						printf("failed\r\n");
					}
					if(SD_ReadDisk(buf2,3,1)==0)	//读3扇区的内容
					{	
						LCD_ShowString(30,190,200,16,16,"USART1 Sending Data...");
						printf("SECTOR 3 DATA:\r\n");
						for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf2[sd_size]);//打印3扇区数据    	   
						printf("\r\nDATA ENDED\r\n");
						LCD_ShowString(30,190,200,16,16,"USART1 Send Data Over!");
					}
					myfree(0,buf2);//释放内存
				}
			}
		t++;
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;
			t=0;
		}
	}   
}





