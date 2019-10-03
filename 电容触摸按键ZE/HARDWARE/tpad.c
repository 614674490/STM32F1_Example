#include "tpad.h"
#include "delay.h"

#define TPAD_ARR_MAX_VAL 0XFFFF;         //最大ARR值

vu16 tpad_default_val=0;                //空载时 计数器计算的充电时间

//输入捕获初始化
void TIM5_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//初始化IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;     //浮空输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化定时器
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=	arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	//设置输入捕获参数  TIM5 CH2
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=0X03;                  //8个定时器时钟周期滤波
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;   //检测到上升沿说明充电完成
	TIM_ICInitStructure.TIM_ICPrescaler=0;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //TI2
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	
	TIM_Cmd(TIM5,ENABLE);                                    //使能定时器5
}
//按键触摸初始化
//0:初始化成功 1:初始化失败
u8 TAPD_Init(u16 psc)
{
	u16 buf[10];                              //存储10次充电时间 计算平均值
	u16 temp;
	u8 i,j;                                  //用于循环 升序排列
	TIM5_Cap_Init(0XFFFF,psc-1);       //以1MHZ的频率计数
	for(i=0;i<10;i++)
	{
		buf[i]=TPAD_Get_Val();
		delay_ms(10);
	}
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++) temp+=buf[i];
	tpad_default_val=temp/6;
	if(tpad_default_val>0xFFFF/2) return 1;     //如果初始化遇到超过TPAD_ARR_MAX_VAL/2 说明初始化失败
	return 0;                                   //初始化成功返回0
}

//复位一次
void TPAD_Reset(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;     //推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PA.1输出0,放电
	
	delay_ms(5);                                 //延时5Ms
	
	TIM_SetCounter(TIM5,0);                         //将计数器的值清0
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC2);    //清除中断标志位
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;     //设置浮空输入 等待充电
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//得到定时器捕获值
//如果超时,则直接返回定时器的计数值.
u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	while(TIM_GetITStatus(TIM5,TIM_IT_CC2)==RESET)    // 等待捕获上升沿
	{
		if(TIM_GetCounter(TIM5)>0xFFFF-500) return TIM_GetCounter(TIM5);  //如果超时 返回CNT
	}
	return TIM_GetCapture2(TIM5);	                         //返回充电时间
}

//读取n次,取最大值
//n：连续获取的次数
//返回值：n次读数里面读到的最大读数值
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();
		if(temp>res) res=temp;
	}
	return res;
}

//扫描触摸按键
//mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
//返回值:0,没有按下;1,有按下;										  
#define TPAD_GATE_VAL 	100	//触摸的门限值,也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;
	u8 res=0;
	u8 sample=3;                        //默认采样次数为3
	u16 rval;
	if(mode)
	{
		keyen=0;
		sample=6;                         //支持连续按，设置采样次数为6次
 	}
	rval=TPAD_Get_MaxVal(sample);      //经过采样 获取最大值
	if(rval>tpad_default_val+TPAD_GATE_VAL)    //大于充电时长 按键有效
	{
		if(keyen==0) res=1;
		keyen=3;                              //至少要经过三次之后按键采样有效
	}
	if(keyen) keyen--;
	return res;
}

