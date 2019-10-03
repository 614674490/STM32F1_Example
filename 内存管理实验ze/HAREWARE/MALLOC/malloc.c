#include "malloc.h"

//ÄÚ´æ³Ø(32×Ö½Ú¶ÔÆë)
__align(32) u8 mem1base[MEM1_MAX_SIZE];													//ÄÚ²¿SRAMÄÚ´æ³Ø
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//Íâ²¿SRAMÄÚ´æ³Ø
//ÄÚ´æ¹ÜÀí±í
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//ÄÚ²¿SRAMÄÚ´æ³ØMAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//Íâ²¿SRAMÄÚ´æ³ØMAP
//ÄÚ´æ¹ÜÀí²ÎÊı	   
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};			//ÄÚ´æ±í´óĞ¡
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};						//ÄÚ´æ·Ö¿é´óĞ¡
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};								//ÄÚ´æ×Ü´óĞ¡


//ÄÚ´æ¹ÜÀí¿ØÖÆÆ÷
struct _m_mallco_dev mallco_dev=
{
	my_mem_init,				//ÄÚ´æ³õÊ¼»¯
	my_mem_perused,				//ÄÚ´æÊ¹ÓÃÂÊ
	mem1base,mem2base,			//ÄÚ´æ³Ø
	mem1mapbase,mem2mapbase,	//ÄÚ´æ¹ÜÀí×´Ì¬±í
	0,0,  		 				//ÄÚ´æ¹ÜÀíÎ´¾ÍĞ÷
};

//¸´ÖÆÄÚ´æ
//*des:Ä¿µÄµØÖ·
//*src:Ô´µØÖ·
//n:ĞèÒª¸´ÖÆµÄÄÚ´æ³¤¶È(×Ö½ÚÎªµ¥Î»)
void mymemcpy(void *des,void *src,u32 n)
{
	u8 *xdes=des;
	u8 *xsrc=src;
	while(n--) *xdes++=*xsrc++;
}

//ÉèÖÃÄÚ´æ
//*s:ÄÚ´æÊ×µØÖ·
//c :ÒªÉèÖÃµÄÖµ
//count:ĞèÒªÉèÖÃµÄÄÚ´æ´óĞ¡(×Ö½ÚÎªµ¥Î»)
void mymemset(void *s,u8 c,u32 count)  
{
	u8 *xs=s;
	while(count--) *xs++=c;
}

//ÄÚ´æ¹ÜÀí³õÊ¼»¯  
//memx:ËùÊôÄÚ´æ¿é   =0/1
void my_mem_init(u8 memx)  
{                                                            //ÄÚ´æ×´Ì¬±íÊı¾İÇåÁã
	mymemset(mallco_dev.membase[memx],0,memtblsize[memx]*2);   //ÄÚ´æ±íÒ»¸öµ¥ÔªÎªu16 Á½¸ö×Ö½Ú Òò´ËÒª³Ë2
	mymemset(mallco_dev.membase[memx],0,memsize[memx]);        //ÄÚ´æ³ØËùÓĞÊı¾İÇåÁã
	mallco_dev.memrdy[memx]=1;                                  //ÄÚ´æ¹ÜÀí³õÊ¼»¯OK
}

//»ñÈ¡ÄÚ´æÊ¹ÓÃÂÊ
//memx:ËùÊôÄÚ´æ¿é
//·µ»ØÖµ:Ê¹ÓÃÂÊ(0~100)
u8 my_mem_perused(u8 memx)  
{
	u32 used=0;
	u32 i;
	for(i=0;i<memtblsize[memx];i++)
	{
		if(mallco_dev.memmap[memx][i]) used++;
	}
	return (used*100)/(memtblsize[memx]);
}

//ÄÚ´æ·ÖÅä(ÄÚ²¿µ÷ÓÃ)
//memx:ËùÊôÄÚ´æ¿é
//size:Òª·ÖÅäµÄÄÚ´æ´óĞ¡(×Ö½Ú)
//·µ»ØÖµ:0XFFFFFFFF,´ú±í´íÎó;ÆäËû,ÄÚ´æÆ«ÒÆµØÖ· 
u32 my_mem_malloc(u8 memx,u32 size)  
{
	signed long offset=0;
	u32 nmemb;                  //ËùĞèµÄÄÚ´æ¿éÊı
	u32 cmemb=0;                //Á¬ĞøµÄ¿ÕÄÚ´æ¿éÊı
	u32 i;
	if(!mallco_dev.memrdy[memx]) mallco_dev.init(memx);    //Î´³õÊ¼»¯ ÏÈ³õÊ¼»
	if(size==0)return 0XFFFFFFFF;//²»ĞèÒª·ÖÅä
	nmemb=size/memblksize[memx];              //»ñÈ¡ËùĞèµÄÁ¬ĞøÄÚ´æ¿éÊı
	if(size%memblksize[memx]) nmemb++;        //ÓĞÓàÊı ÊıÄ¿»¹Òª¼Ó1
	for(offset=memtblsize[memx]-1;offset>=0;offset--)    //´Ó¶¥²¿¿ªÊ¼ ËÑË÷Õû¸öÄÚ²¿´æ´¢Çø
	{
		if(!mallco_dev.memmap[memx][offset]) cmemb++;       //Á¬Ğø¿ÕÄÚ´æ¿éÔö¼Ó
		else cmemb=0;                               //Á¬Ğø¿ÕÄÚ´æ¿éÇåÁã
		if(cmemb==nmemb)                            //ÕÒµ½nmemb¸öÁ¬Ğø¿ÕÄÚ´æ¿é
		{
			for(i=0;i<nmemb;i++)                     //±ê×¢ÄÚ´æ¿é·Ç¿Õ  ´ËÊ±offsetÎª×îºóÒ»¸öÄÚ´æ¿éµÄÆ«ÒÆµØÖ·
			{
				mallco_dev.memmap[memx][offset+i]=nmemb;  //ÄÚ´æ¿éÍ¨¹ıµØÖ·µİÔö
			}
			return (offset*memblksize[memx]);                //·µ»ØÆ«ÒÆµØÖ·
		}
	}
	return 0XFFFFFFFF;                                  //Î´ÕÒµ½·ûºÏ·ÖÅäÌõ¼şµÄÄÚ´æ¿é  
}

//ÊÍ·ÅÄÚ´æ(ÄÚ²¿µ÷ÓÃ) 
//memx:ËùÊôÄÚ´æ¿é
//offset:ÄÚ´æµØÖ·Æ«ÒÆ
//·µ»ØÖµ:0,ÊÍ·Å³É¹¦;1,ÊÍ·ÅÊ§°Ü;  
u8 my_mem_free(u8 memx,u32 offset) 
{
	int i;
	if(!mallco_dev.memrdy[memx])
	{
		mallco_dev.init(memx);    //Î´³õÊ¼»¯ ÏÈ³õÊ¼»¯
		return 1;                //Î´³õÊ¼»¯
	}
	if(offset<memsize[memx])    //Æ«ÒÆÔÚÄÚ´æ³ØÄÚ
	{
		int index=offset/memblksize[memx];       //Æ«ÒÆËùÔÚÄÚ´æ¿éºÅÂë
		int nmemb=mallco_dev.memmap[memx][index];   //ÄÚ´æ¿éÊıÁ¿
		for(i=0;i<nmemb;i++)                    //ÄÚ´æ¿éÇåÁã
		{
			mallco_dev.memmap[memx][index+i]=0;
		}
		return 0;
	}
	else return 2;                         //Æ«ÒÆ³¬Çø
}
	
//ÊÍ·ÅÄÚ´æ(Íâ²¿µ÷ÓÃ) 
//memx:ËùÊôÄÚ´æ¿é
//ptr:ÄÚ´æÊ×µØÖ· 
void myfree(u8 memx,void *ptr)  
{
	u32 offset;
	if(ptr==NULL) return;  //µØÖ·Îª0
  offset=(u32)ptr-(u32)mallco_dev.membase[memx];     
   my_mem_free(memx,offset);	//ÊÍ·ÅÄÚ´æ    
}

//·ÖÅäÄÚ´æ(Íâ²¿µ÷ÓÃ)
//memx:ËùÊôÄÚ´æ¿é
//size:ÄÚ´æ´óĞ¡(×Ö½Ú)
//·µ»ØÖµ:·ÖÅäµ½µÄÄÚ´æÊ×µØÖ·.
void *mymalloc(u8 memx,u32 size) 
{
	u32 offset;
	offset=my_mem_malloc(memx,size);
	if(offset==0xffffffff) return NULL;
	else return (void*)((u32)mallco_dev.membase[memx]+offset);
}
//ÖØĞÂ·ÖÅäÄÚ´æ(Íâ²¿µ÷ÓÃ)
//memx:ËùÊôÄÚ´æ¿é
//*ptr:¾ÉÄÚ´æÊ×µØÖ·
//size:Òª·ÖÅäµÄÄÚ´æ´óĞ¡(×Ö½Ú)
//·µ»ØÖµ:ĞÂ·ÖÅäµ½µÄÄÚ´æÊ×µØÖ·.
void *myrealloc(u8 memx,void *ptr,u32 size)  
{  
    u32 offset;    
    offset=my_mem_malloc(memx,size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//¿½±´¾ÉÄÚ´æÄÚÈİµ½ĞÂÄÚ´æ   
        myfree(memx,ptr);  											  		//ÊÍ·Å¾ÉÄÚ´æ
        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//·µ»ØĞÂÄÚ´æÊ×µØÖ·
    }  
}




