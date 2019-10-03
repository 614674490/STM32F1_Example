#include "wdg.h"

void WDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);             //д��0x5555 ȡ��д����
	
	IWDG_SetPrescaler(prer);                                  //����Ԥ��Ƶϵ�� ȷ��ʱ��
	
	IWDG_SetReload(rlr);                                      //������װ��ֵ   ȷ�����ʱ��
	
	IWDG_ReloadCounter();                                     //Ӧ�ó���ι��
	
	IWDG_Enable();                                            //ʹ�ܿ��Ź�
}




	
