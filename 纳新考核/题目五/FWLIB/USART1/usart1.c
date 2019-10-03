#include "usart1.h"
#include "stm32f10x_usart.h"
#include "led.h"
u8 USART1_RX_BUF[USART1_REC_LEN]; //Êý×éÓÃÀ´´æ´¢½ÓÊÕµ½µÄÊý¾Ý£¬¶øUSART3_REC_LENÎª×î¶àÄÜ½ÓÊÕµÄ×Ö½ÚÏÞ¶È
u16 USART1_RX_STA=0;         		//½ÓÊÕ×´Ì¬±ê¼Ç	 0-14Î»Îª½ÓÊÕ×Ö½ÚÊý£¬15Î»½ÓÊÕÍê³É±êÖ¾Î»
void My_USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);   //´®¿ÚºÍGPIO¿ÚÊ¹ÄÜ
	
	//GPIO¶Ë¿ÚÄ£Ê½ÉèÖÃ
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;    //PA9 TXD ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    //PA10 RXD ¸¡¿ÕÊäÈë
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//´®¿Ú³õÊ¼»¯
	USART_InitStructure.USART_BaudRate=115200;               //ÉèÖÃ²¨ÌØÂÊ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;    //²»ÊÊÓÃÓ²¼þÁ÷
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                      //Ê¹ÄÜ·¢ËÍºÍ½ÓÊÜÄ£Ê½
	USART_InitStructure.USART_Parity=USART_Parity_No;                                //²»Ê¹ÓÃÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                             //ÉèÖÃÍ£Ö¹Î» 1
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                        //ÉèÖÃ×Ö³¤ 8Î»
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);                                                        //Ê¹ÄÜ´®¿Ú1
	
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);               //Ê¹ÄÜ½ÓÊÜÖÐ¶ ÓÃDMAÖÐ¶Ï½ÓÊÕÊý¾ÝÐèÒª°Ñ´ËÖÐ¶Ï¹Ø±Õ
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); //Ê¹ÄÜ´®¿Ú1µÄDMA½ÓÊÕ  
	
	//¿ªÆôÖÐ¶Ï·Ö×é³õÊ¼»¯
	NVIC_InitInitStructure.NVIC_IRQChannel=USART1_IRQn;                              //´®¿Ú1Í¨µÀ
	NVIC_InitInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitInitStructure.NVIC_IRQChannelPreemptionPriority=1;                      //ÇÀÕ¼ÓÅÏÈ¼¶Îª1
	NVIC_InitInitStructure.NVIC_IRQChannelSubPriority=1;                             //ÏìÓ¦ÓÅÏÈ¼¶Îª1
	NVIC_Init(&NVIC_InitInitStructure);
}
//±àÐ´ÖÐ¶Ï´¦Àíº¯Êý
/*void USART1_IRQHandler(void)
{
	u8 res;//ÔÝÊ±»º´æ½ÓÊÕµÄÊý¾Ý
   if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//ÅÐ¶ÏÊÇ·ñÎª´®¿Ú1½ÓÊÕÖÐ¶Ï
	 {
		   
	     res=USART_ReceiveData(USART1);//½ÓÊÕµ½Êý¾Ý·Å½øres
		   USART1_RX_BUF[USART1_RX_STA&0x7FFF]=res;//Êý¾Ý·Å½øÊý×éÖÐ£¬Ôò¿ÉÒÔÓÃµ½mainº¯ÊýÖÐÁË
		   USART1_RX_STA++;                         //×Ö½Ú³¤¶È++
		 if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ	+; 
		  USART1_RX_STA|=0x8000;//´®¿Ú3½ÓÊÕÍê³É
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);//Çå³ý½ÓÊÕÖÐ¶Ï±êÖ¾
		   
	  }
}*/


