#include "iap.h"
#include "usart.h"
#include "delay.h"
#include "stmflash.h"

iapfun jump2app;
u16 iapbuf[1024];

//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u8 *dfu=appbuf;
	u32 fwaddr=appxaddr;   //��¼��ǰ����ʼ��ַ
	for(t=0;t<appsize;t+=2)
	{
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];     //��8bit���ݰ�16bit���ݴ洢
		dfu+=2;            //ƫ�������ֽ�
		iapbuf[i++]=temp;
		if(i==1024)                //д����
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,1024);	
			fwaddr+=2048;       //ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  ������һ��������ʣ������д��
}
//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
	}
}		 















