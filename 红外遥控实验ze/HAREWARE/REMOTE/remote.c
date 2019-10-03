#include "remote.h"
#include "delay.h"
#include "led.h"
//红外遥控初始化 PWM脉冲宽度调制
//设置IO以及定时器2的输入捕获
void Remote_Init(void)    		
{
	//TIM2_CH2->PA1
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	//初始化IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;         //PA1 下拉输入  不用设置速度
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;             //PA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);						       //PA1 上拉
	
	//初始化定时器2 计数器频率为1Mhz 10ms溢出一次
	TIM_TimeBaseStructure.TIM_ClockDivision=0;                 //设置时钟分割 TDTS=0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //设置计数模式 向上计数
	TIM_TimeBaseStructure.TIM_Period=10000;                      //自动重载值
	TIM_TimeBaseStructure.TIM_Prescaler=71;                   //预分频
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//初始化输入捕获通道
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;             //CC1S=01 选择输入端 IC2映射到TI2上 通道2
	TIM_ICInitStructure.TIM_ICFilter=0x03;                     //配置输入滤波器 8个定时器时钟周期滤波
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //设置捕获方式 上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;        //配置输入分频 不分频
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI ;  //选择映射 映射到TI2上 通道2要映射到通道1选择间接，反之直接
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	//中断初始化分组
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);      //允许更新中断和CC2E捕获中断
	
	TIM_Cmd(TIM2,ENABLE);                                   //使能定时器2
}

//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留	
//[4]:标记上升沿是否已经被捕获								   
//[3:0]:溢出计时器
u8 	RmtSta=0;	  	  
u16 Dval;		//下降沿时计数器的值
u32 RmtRec=0;	//红外接收到的数据	   		    
u8  RmtCnt=0;	//按键按下的次数	  
//定时器4中断服务程序	 
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)          //产生更新中断
	{
		if(RmtSta&0x80)                                       //上次有数据接收到
		{
			RmtSta&=~0x10;                                      //取消上升沿捕获标志
			if((RmtSta&0x0F)==0x00)  RmtSta|=1<<6;               //标记已经完成一次按键信息的采集
			if((RmtSta&0x0F)<14)      RmtSta++;
			else
			{
				RmtSta&=~(1<<7);                                  //清空引导标志
				RmtSta&=0xF0;                                     //计数器清零
			}
		}
	}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)          //产生捕获中断
	{
		if(RDATA)                                             //上升沿捕获
		{
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);   //设置为下降沿捕获
			TIM_SetCounter(TIM2,0);                                 //清空计数器
			RmtSta|=0x10;                                           //标记上升沿已经捕获
		}
		else                                                      //下降沿捕获
		{
			Dval=TIM_GetCapture2(TIM2);                            //获取计数器的值
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);   //设置为上升沿捕获
			if(RmtSta&0x10)                                       //捕获到上升沿
			{
				if(RmtSta&0x80)                                          //接收到了引导码
				{
					if(Dval>300&&Dval<800)         //高电平为560us
					{
						RmtRec<<=1;                   //左移一位
						RmtRec|=0;                   //逻辑0
					}
					else if(Dval>1400&&Dval<1800)    //高电平为1680us
					{
						RmtRec<<=1;                   //左移一位
						RmtRec|=1;                   //逻辑1
					}
					else if(Dval>2200&&Dval<2600)        //2500us 按键增加值的信息
					{
						RmtCnt++;                           //按键次数自加
						RmtSta&=0xF0;                       //清空计数器
					}
				}
				else if(Dval>4200&&Dval<4700)   //正在接收引导码  4500ms
				{
					RmtSta|=1<<7;                 //标记接收到引导码；
					RmtCnt=0;                     //清空按键次数
				}
			}
			RmtSta&=~(1<<4);
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC2);         //清楚中断标志位
}

//处理红外键盘
//返回值:
//	 0,没有任何按键按下
//其他,按下的按键键值.
u8 Remote_Scan(void)
{
	u8 sta=0;
	u8 t1,t2;
	if(RmtSta&(1<<6))    //得到了一个按键的所有信息
	{
		t1=RmtRec>>24;      //得到地址码
		t2=(RmtRec>>16)&0xff;   //得到地址反码
		if((t1==(u8)~t2)&&(t1==REMOTE_ID))       //检测遥控ID及地址码
		{
			t1=RmtRec>>8;                              //控制码校验
			t2=RmtRec;
			if(t1==(u8)~t2) sta=t1;    //键值正确
		}
		if((sta==0)||(RmtSta&0x80)==0)   //按键数据错误 遥控未被按下
		{
			RmtSta&=~(1<<6);                //清除接收到有效按键的标志
			RmtCnt=0;                       //清空按键次数
		}
	}
	return sta;
}




