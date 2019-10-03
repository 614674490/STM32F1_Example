#include "gsm.h"
#include "usart2.h" 
#include "delay.h"
#include "usart.h"
#include "gps.h"
#include "delay.h"
#include "lcd.h"
#include "string.h"
//----------------------------------------------------------------------------------------------------------------------------------------------
//���ַ����в���ָ���ַ�
//������sdat--Դ�ַ�����tdat--Ҫ���ҵ�ָ���ַ���num--Դ�ַ���Ҫ���ҵ����ݸ�����num2--ָ���ַ��������ݸ���
//����ֵ��1-1000:���ҳɹ���������ʼ�ֽ�����λ�ã�0:δ�ҵ�
u16 FindStr(u8 *sdat,u8 *tdat,u16 num,u16 num2)
{
	u16 i,j,k=0;
	for(i=0;i<num;i++)
	{
		k=0;
		if(*sdat==*tdat)
		{
			k=1;
			for(j=1;j<num2;j++)
			{
					if(*(sdat+j)==*(tdat+j))
							k++;
					else break;
			}
		}
		if(k==num2)
			break;
		sdat++;
	}
	if(k==num2)
		return (i+1);
	else return 0;
}

//�ȴ������ַ�������ʱʱ��10��
//����ֵ��0--��ȷ������--����
u8 WaitForStr(u8 *p,u16 num,u8 timeout)
{
	u8 temp;
	u8 i=0,j=0,tim=0,err=0;
	while(j<timeout)
	{
		if(USART2_RX_STA&0x8000)
		{
			temp=USART2_RX_STA&0x7fff;  //�õ����ݳ���
			USART2_RX_STA=0;
			i=FindStr(USART2_RX_BUF,p,temp,num);
			if(i>0)    
				break;            
		}
		delay_ms(100);
		tim++;
		if(tim>10)     //��ʱ
		{
			j++;
			tim=0;            
		}
	}
	if(j>=timeout)
		err=1;
	for(i=0;i<200;i++)
	{
		USART2_RX_BUF[i]=0;
	}
	return err;
} 

//�ȴ�A9G��ʼ�����
//0����� 1��ʧ��
u8 A9G_Init(void)
{
	u8 temp;
	temp=WaitForStr("+CREG: 1",8,20);
	return temp;
}
//GPS��ʼ��
//����ֵ 0���ɹ� 1��ʧ��
u8 GPS_Init(void)
{
	u8 temp;
	temp=GPS_ON();          //����GPS
	if(temp>0) return temp;
	SendAT("AT+GPSMD=1",10);   //����ΪGPSģʽ
	temp=WaitForStr("GPSMD",5,2); 
	if(temp>0) return temp;
	return temp;
}

//GPRS����
////����ֵ 0���ɹ� 1��ʧ��
u8 SIM_Connect(void)
{
	u8 temp;
	SendAT("AT+CCID",7);   //��ѯSIM���� ���SIM�Ƿ����
	temp=WaitForStr("CCID",4,5); 
	if(temp>0) {printf("\r\nCCID Failed\r\n");return temp;}
	
	SendAT("AT+CREG?",8);   //SIM���Ƿ�ע������
	temp=WaitForStr("1,1",3,5);           //��ע�᱾������
	if(temp>0) temp=WaitForStr("1,5",3,5);   //ע������ ��������״̬
	if(temp>0) {printf("\r\nCREG Failed\r\n");return temp;}
	
	SendAT("AT+CGATT=1",10);   //�������� �����Ҫ���� ��ָ���ʡȥ
	temp=WaitForStr("CGATT",5,5); 
	if(temp>0) {printf("\r\nCGATT Failed\r\n");return temp;}
	
	SendAT("AT+CGDCONT=1,\"IP\",\"CMENT\"",25);   //����PDP����
	temp=WaitForStr("OK",2,5); 
	if(temp>0) {printf("\r\nCGDCONT Failed\r\n");return temp;}
	
	SendAT("AT+CGACT=1,1",12);   //����PDP
	temp=WaitForStr("OK",2,5); 
	if(temp>0) {printf("\r\nCGACT Failed\r\n");return temp;}
	
	SendAT("AT+CIPSTATUS?",13);                 //��ѯIP��·״̬
	temp=WaitForStr("GPRSACT",7,5);
	if(temp>0) {printf("\r\nConnect Failed\r\n");return temp;}
	return temp;
}

//0:�ɹ� 1��ʧ��
u8 SendGPSData(u8 *dat)          //����GPS��Ϣ������
{
	u8 i,j=0;
	
	while(j<3)
	{    	
		//����TCP����
		j++;
    SendAT("AT+CIPSTART=\"TCP\",\"119.23.14.83\",80",39);    
    i=WaitForStr("CONNECT OK",10,20);              //AT+CIPSTART="TCP","119.23.14.83/gps",80 OK CONNECT OK
		if(i>0) //δ�ܳɹ�����
		{
			printf("\r\nCONNECT Failed\r\n");
	    SIM_Connect();
		}
		else 
		{
			printf("\r\nCONNECT OK\r\n");
			break;
		}
	}
	if(i==0)  //�ɹ�����
	{
		delay_ms(200);
		//����HTTP����
		sprintf((char*)ATData,"?%s",dat);
		printf("\r\n%s\r\n",ATData);                                  //������ʾ���͵�����
		SendAT("AT+CIPSEND",10);        
		if(WaitForStr(">",1,5)) printf("\r\nWait......\r\n");              //�ȴ�����������
		else SendATData(ATData);
		
		i=WaitForStr("OK",2,5);                           //��������Ƿ���
        
		if(i==0)
			LCD_ShowString(30,80,200,16,16,"Send OK   ");
		else LCD_ShowString(30,80,200,16,16,"Send Error");
		SendAT("AT+CIPCLOSE",11);                             //�ر���·
		if(WaitForStr("OK",2,5)) printf("Close Failed");
		else    printf("Close OK");
		return 0;
	}
	else                                 //δ�ɹ�����
	{
		printf("\r\nCONNECT Failed\r\n");
		return 1;
	}
}

//HTTPGET ����GPS��Ϣ
void HttpGet(u8 *dat)
{
	u16 num;
	sprintf((char*)ATData,"AT+HTTPGET=\"http://119.23.14.83?%s\"",dat);
	num=strlen((char *)ATData);
	printf("\r\n%s\r\n",ATData);                      //������ʾ��Ҫ���͵�����
	SendAT(ATData,num);
	if(WaitForStr("OK",2,10)) 
  {
	  LCD_ShowString(30,80,200,16,16,"Send Error   ");
		printf("\r\nSend Error\r\n");
	  if(SIM_Connect()) {LCD_ShowString(30,40,200,16,16,"SIM Failed    ");}      //SIM��������
		else  {printf("\r\nInternet Error\r\n");LCD_ShowString(30,40,200,16,16,"Internet Error");}                     //����������
	}
	else  
	{
		LCD_ShowString(30,80,200,16,16,"Send OK       ");
		printf("\r\nSend OK\r\n");
		SendAT("EXIT",4);
		if(WaitForStr("+CME ERROR",10,10)) printf("\r\nExit Failed\r\n");
		else printf("\r\nExit OK\r\n");
	}

}
//����GPS
u8 GPS_ON(void)
{
	u8 temp;
	SendAT("AT+GPS=1",8);
	temp=WaitForStr("OK",2,2);	
	return temp;
}
u8 GPS_OFF(void)
{
	u8 temp;
	SendAT("AT+GPS=0",8);
	temp=WaitForStr("OK",2,2); 
	return temp;
}
void A9G_Check(void)
{
	SendAT("AT",2);
	if(WaitForStr("OK",2,2))
	{
		SendAT("AT+RST=1",8);        //APG��λ
		if(WaitForStr("Init",4,3)==0)
			printf("\r\nRST...\r\n");
		Init();
	}
}
	
void Init(void)
{
	while(A9G_Init())
	{
		 LCD_ShowString(30,20,200,16,16,"A9G Failed");
		 printf("\r\nA9G Init...\r\n");
	}
	LCD_ShowString(30,20,200,16,16,"A9G OK    ");
	printf("\r\nAPG Init OK\r\n");
	while(SIM_Connect())
	{
		 LCD_ShowString(30,40,200,16,16,"SIM Failed");
	}
	LCD_ShowString(30,40,200,16,16,"SIM OK    ");
	printf("\r\nSIM Init OK\r\n");
	while(GPS_Init())
	{
		 LCD_ShowString(30,60,200,16,16,"GPS Failed");
		 printf("\r\nGPS Init...\r\n");
	}
	LCD_ShowString(30,60,200,16,16,"GPS OK    ");
	printf("\r\nGPS Init OK\r\n");
	LCD_ShowString(30,80,200,16,16,"Wait Send ");   //��ʾ�ȴ�����
}





