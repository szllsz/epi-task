#include <REGX52.H>
#include "Delay.h"
//#include "key_multi.h"
#include "UART.h"

#define uchar unsigned char
#define uint unsigned int
#define length 2

sbit PWMIN1 = P2^6;
sbit PWMIN2 = P2^7;
sbit Key1 = P1^3;
sbit Key5 = P1^7;

uchar state,key,High1,Low1,flag1,High5,Low5,flag5;
uchar f1,DR1,f2,DR2,QS,HS;
uchar time11,time12,time13,time14,lasts1,nows1,time21,time22,time23,time24,lasts2,nows2;
uchar i,temp1,temp2,display1[length],display2[length],mood21,mood22,mood11,mood12;
uint counter;

uchar HighRH1,HighRL1,LowRH1,LowRL1,HighRH2,HighRL2,LowRH2,LowRL2,mood1,mood2;
uchar count1,count2,state2;

uchar code led_code1[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};//原
uchar code led_code2[] = {0x50, 0x5F, 0x32, 0x16, 0x1D, 0x94, 0x90, 0x5E, 0x10, 0x14, 0xEF};//0-9 .  新

void Timer0_Isr(void) interrupt 1
{
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
//	counter++;
	
	time11++;		
	lasts1 = nows1;				//第一列波测波
	nows1 = PWMIN1;
	switch(mood1)
	{
		case 0:
		{
			if(lasts1 == 0 && nows1 == 1)
			{
				time12 = time11;
				mood1 = 1;
			}
		}break;
		case 1:
		{
			if(lasts1 == 1 && nows1 == 0 )
			{
				time13 = time11;
				mood1 = 2;
			}
		}break;
		case 2:
		{
			if(lasts1 == 0 && nows1 == 1 )
			{					
				time14 = time11;
				mood1 = 3;
			}
		}break;
	}
	
	time21++;		
	lasts2 = nows2;				//第二列波测波
	nows2 = PWMIN2;
	switch (mood2)
	{
		case 0:
		{
			if(lasts2 == 0 && nows2 == 1)
			{
				time22 = time21;
				mood2 = 1;
			}
		}break;
		case 1:
		{
			if(lasts2 == 1 && nows2 == 0 )
			{
				time23 = time21;
				mood2 = 2;
			}
		}break;
		case 2:
		{
			if(lasts2 == 0 && nows2 == 1 )
			{					
				time24 = time21;
				mood2 = 3;
			}
		}break;
	}
}

void Timer0_Init(void)		//1000微秒@11.0592MHz
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	PT0 = 1;
}

void Nixie(unsigned char Location, Number)			//数码管显示
{
//	switch(state)
//	{
//		case 0:
//		{
			switch(Location)
			{
				case 1:P3_5=0;P3_4=0;P3_3=0;P3_2=1;break;
				case 2:P3_5=0;P3_4=0;P3_3=1;P3_2=0;break;
				case 3:P3_5=0;P3_4=1;P3_3=0;P3_2=0;break;
				case 4:P3_5=1;P3_4=0;P3_3=0;P3_2=0;break;
			}
			P0 = led_code2[Number];
			Delay(1); 
			P0 = 0xFF;
//		}break;
//		case 1:
//		{
//			switch(Location)
//			{
//				case 1:P2_4=1;P2_3=1;P2_2=1;break;
//				case 2:P2_4=1;P2_3=1;P2_2=0;break;
//				case 3:P2_4=1;P2_3=0;P2_2=1;break;
//				case 4:P2_4=1;P2_3=0;P2_2=0;break;
//				case 5:P2_4=0;P2_3=1;P2_2=1;break;
//				case 6:P2_4=0;P2_3=1;P2_2=0;break;
//				case 7:P2_4=0;P2_3=0;P2_2=1;break;
//				case 8:P2_4=0;P2_3=0;P2_2=0;break;
//			}
//			P0=led_code1[Number];
//			Delay(1); 
//			P0=0x00;
//		}break;
//	}
}

void Lvbo(uchar *n,num)
{
	for(i = 1; i < num + 1; i++)
	{
		if(*n > (10 * i - 5) && *n < (10 * i + 5)) *n = 10 * i;
	}
}

void main()
{
	EA = 1;
	UART_Init();
	Timer0_Init();
	f1 = 10;
	DR1 = 50;
	f2 = 20;
	DR2 = 60;
	QS = f1;
	HS = DR1;
	while(1)
	{
		if(mood1 == 3)
		{
			f1 = 1000 / (time14 - time12);
			DR1 = 100 * (time13 - time12) / (time14 - time12);
			time11 = 0;
			mood1 = 0;	
		}
		if(mood2 == 3)
		{
			f2 = 1000 / (time24 - time22);
			DR2 = 100 * (time23 - time22) / (time24 - time22);
			time21 = 0;
			mood2 = 0;	
		}
		Lvbo(&f1,5);Lvbo(&DR1,9);Lvbo(&f2,5);Lvbo(&DR2,9);
		
		if(Key1 == 0 && flag1 == 0)															
		{
			High1++;
			if(High1 >= 1)				//按下检测>=10ms
			{
				High1 = 0;
				flag1 = 1;
			}
		}
		else if(Key1 != 0 && flag1 == 1)
		{
			Low1++;
			if(Low1 >= 1)				//松开检测>=10ms
			{
				key++;
				key %= 2;
				P2_0 =~ P2_0;
				flag1 = 0;
			}
		}
		if(!key) {QS = f1;HS = DR1;}
		else {QS = f2;HS = DR2;}
		
		temp1 = QS;
		temp2 = HS;
		for(i = 0; i < length; i++)									//前两位显示
		{
			display1[length - i - 1] = temp1 % 10;
			temp1 /= 10;
			Nixie(i + 1, display1[i]);
		}
		for(i = 0; i < length; i++)									//后两位显示
		{
			display2[length - i - 1] = temp2 % 10;
			temp2 /= 10;
			Nixie(i + 3, display2[i]);
		}
//		if(counter >= 1000)
//		{
//			counter = 0;
////			P2_0 =~ P2_0;
//			UART_SendByte1('c');UART_SendByte1('h');UART_SendByte1('1');UART_SendByte1(':');UART_SendByte1('f');UART_SendByte1('=');
//			UART_SendByte2(f1);
//			UART_SendByte1(',');UART_SendByte1('d');UART_SendByte1('=');
//			UART_SendByte2(DR1);
//			UART_SendByte1(';');
//			UART_SendByte1('c');UART_SendByte1('h');UART_SendByte1('2');UART_SendByte1(':');UART_SendByte1('f');UART_SendByte1('=');
//			UART_SendByte2(f2);
//			UART_SendByte1(',');UART_SendByte1('d');UART_SendByte1('=');
//			UART_SendByte2(DR2);
//			UART_SendByte1(';');
//		}
	}
}
