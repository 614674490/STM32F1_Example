#include "display.h"

u16 CurrentColor=RED;//画笔颜色
u16 ColorList[COLOR_COUNT]={BLACK,BLUE,BRED,YELLOW,GBLUE,RED,GREEN,WHITE};//调色板颜色列表

//画调色板
void Draw_Palette(void){
	u8 i=0;
	//u32 u32_t=0;
	LCD_Fill(5,5,35,35,RED);
	LCD_DrawRectangle(5,5,35,35);
	
	for(i=0;i<COLOR_COUNT;i++){
		LCD_Fill(i*24+40,5,i*24+40+20,25,ColorList[i]);
		//LCD_DrawRectangle(5,5,35,35);
	}
}
//画画布
void Draw_Canvas(void){
	POINT_COLOR=YELLOW;//设置字体为红色
	LCD_DrawRectangle(0,40,240-1,320-1);
	LCD_DrawRectangle(1,41,240-2,320-2);
	POINT_COLOR=BLACK;//设置字体为红色
	LCD_DrawRectangle(2,42,240-3,320-3);
}


//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);	//清屏   
 	POINT_COLOR=BLUE;	//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;	//设置画笔蓝色 
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   

//电阻触摸屏测试函数
void rtp_test(void)
{
	u8 key;
	u8 i=0;
  Draw_Palette();  //画调色板
	Draw_Canvas();   //画画布	
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(tp_dev.y[0]<45){   //在调色板区域
				if(tp_dev.x[0]>45 && tp_dev.x[0]<(240-2) && tp_dev.y[0]>5 && tp_dev.y[0]<25){//颜色选择区,更换笔刷颜色
					i=(tp_dev.x[0]-45)/24;     //获取所选择的调色板颜色索引号
					CurrentColor=ColorList[i];
					LCD_Fill(5,5,35,35,CurrentColor);
					POINT_COLOR=BLACK;
					LCD_DrawRectangle(5,5,35,35);
				}
				else if(tp_dev.x[0]>5 && tp_dev.x[0]<35 && tp_dev.y[0]>5 && tp_dev.y[0]<35){//笔刷颜色指示区,点击治理清空画布
					LCD_Fill(3,43,240-4,320-4,WHITE);
				}
			}
			else if(tp_dev.y[0]<315){                                 //画布区域
				TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],CurrentColor);		//画图
			}
		}    
		if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
		{
			LCD_Clear(WHITE);//清屏
		  TP_Adjust();  	//屏幕校准  
			TP_Save_Adjdata();
			NVIC_SystemReset();  //校准完毕 , 重启,
		}
		i++;
		if(i%20==0)LED0=!LED0;
	}
}
//5个触控点的颜色												 
const u16 POINT_COLOR_TBL[CT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
//电容触摸屏测试函数
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//记录最后一次的数据 
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<CT_MAX_TOUCH;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<16)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0)LED0=!LED0;
	}	
}

