#ifndef _IAP_H
#define _IAP_H
#include "sys.h"

typedef void (*iapfun) (void);  //����һ���������͵Ĳ���

#define FLASH_APP1_ADDR 0x08010000    //��һ��Ӧ�ó���Ĵ�ŵ�ַ��FLASH��

void iap_load_app(u32 appxaddr);			//ִ��flash�����app����
void iap_load_appsram(u32 appxaddr);		//ִ��sram�����app����
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//��ָ����ַ��ʼ,д��bin






#endif


