#include <REGX52.H>
#include "Delay.h"
#include "key_multi.h"
#include "UART.h"
#include "AT24C02.h"

#define uchar unsigned char
#define uint unsigned int
#define length 2

sbit PWMOUT1 = P2^6;
sbit PWMOUT2 = P2^7;
sbit Key1 = P3^1;
sbit Key5 = P3^3;

uchar f1,DR1,f2,DR2,i,state = 1,Loc,Num,t,p,count1,count2,QS,HS,temp1,temp2;									//f：频率  DR：占空比
uchar flag1,High1,Low1, flag3,High3,Low3, flag4,High4,Low4, flag5,High5,Low5;
uchar display1[length],display2[length];
uchar HighRH1,HighRL1,LowRH1,LowRL1,HighRH2,HighRL2,LowRH2,LowRL2,mood1,mood2;
uint counter;

uchar state2,key;
uchar time11,time12,time13,time14,lasts1,nows1,time21,time22,time23,time24,lasts2,nows2,mood21,mood22,mood11,mood12;

uchar code led_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Timer0_Isr(void) interrupt 1
{	
	if(!state2)
	{
		if(mood1 == 0)
		{
			if(PWMOUT1 == 1)
			{
				PWMOUT1 = 0;
				TH0 = LowRH1;				
				TL0 = LowRL1;
			}
			else
			{
				PWMOUT1 = 1;
				TH0 = HighRH1;				
				TL0 = HighRL1;
			}
		}
		else
		{
			if(PWMOUT1 == 1)
			{
				if(count1 == 10)
				{
					count1 = 0;
					TH0 = LowRH1;				
					TL0 = LowRL1;
					PWMOUT1 = 0;
				}
				else
				{
					TH0 = HighRH1;				
					TL0 = HighRL1;
					count1++;
				}
			}
			else
			{

				if(count1 == 10)
				{
					count1 = 0;
					TH0 = HighRH1;				
					TL0 = HighRL1;
					PWMOUT1 = 1;
				}
				else
				{
					TH0 = LowRH1;				
					TL0 = LowRL1;
					count1++;
				}
			}	
		}
	}
	else
	{
		TL0 = 0x66;				//设置定时初始值
		TH0 = 0xFC;				//设置定时初始值
		counter++;
		
		time11++;		
		lasts1 = nows1;				//第一列波测波
		nows1 = PWMOUT1;
		switch(mood11)
		{
			case 0:
			{
				if(lasts1 == 0 && nows1 == 1)
				{
					time12 = time11;
					mood11 = 1;
				}
			}break;
			case 1:
			{
				if(lasts1 == 1 && nows1 == 0 )
				{
					time13 = time11;
					mood11 = 2;
				}
			}break;
			case 2:
			{
				if(lasts1 == 0 && nows1 == 1 )
				{					
					time14 = time11;
					mood11 = 3;
				}
			}break;
		}
		
		time21++;		
		lasts2 = nows2;				//第二列波测波
		nows2 = PWMOUT2;
		switch (mood12)
		{
			case 0:
			{
				if(lasts2 == 0 && nows2 == 1)
				{
					time22 = time21;
					mood12 = 1;
				}
			}break;
			case 1:
			{
				if(lasts2 == 1 && nows2 == 0 )
				{
					time23 = time21;
					mood12 = 2;
				}
			}break;
			case 2:
			{
				if(lasts2 == 0 && nows2 == 1 )
				{					
					time24 = time21;
					mood12 = 3;
				}
			}break;
		}
	}
}

void Timer1_Isr(void) interrupt 3
{
	if(mood2 == 0)
	{
			if(PWMOUT2 == 1)
			{
				TH1 = LowRH2;				
				TL1 = LowRL2;
				PWMOUT2 = 0;
			}
			else
			{
				TH1 = HighRH2;				
				TL1 = HighRL2;
				PWMOUT2 = 1;
			}
	}
	else
	{
		if(PWMOUT2 == 1)
		{
			if(count2 == 10)
			{
				count2 = 0;
				TH0 = LowRH2;				
				TL0 = LowRL2;
				PWMOUT2 = 0;
			}
			else
			{
				TH0 = HighRH2;				
				TL0 = HighRL2;
				count2++;
			}
			
		}
		else
		{
			if(count2 == 10)
			{
				count2 = 0;
				TH0 = HighRH2;				
				TL0 = HighRL2;
				PWMOUT2 = 1;
			}
			else
			{
				TH0 = LowRH2;				
				TL0 = LowRL2;
				count2++;
			}
		}	
	}
}

void Lvbo(uchar *n,num)
{
	for(i = 1; i < num + 1; i++)
	{
		if(*n > (10 * i - 5) && *n < (10 * i + 5)) *n = 10 * i;
	}
}

void pwm(uchar fr1, DC1, fr2, DC2)
{
	uint high1, low1, highload1, lowload1;
	uint high2, low2, highload2, lowload2;
	unsigned long tmp1, tmp2;
	
	tmp1 = (921600) / fr1;				//周期 11059200 / 12
	tmp2 = (921600) / fr2;				//周期 11059200 / 12
	if(fr1 == 10 && DC1 == 80)
	{
		tmp1 /= 10;	
		mood1 = 1;
	}
	else if(fr1 == 10 && DC1 == 90)
	{
		tmp1 /= 10;	
		mood1 = 1;
	}
	else if(fr1 == 10 && DC1 == 10)
	{
		tmp1 /= 10;	
		mood1 = 1;
	}
	else if(fr1 == 10 && DC1 == 20)
	{
		tmp1 /= 10;	
		mood1 = 1;
	}
	else mood1 = 0;
	
	if(fr2 == 10 && DC2 == 80)
	{
		tmp2 /= 10;	
		mood2 = 1;
	}
	else if(fr2 == 10 && DC2 == 90)
	{
		tmp2 /= 10;	
		mood2 = 1;
	}
	else if(fr2 == 10 && DC2 == 10)
	{
		tmp2 /= 10;	
		mood2 = 1;
	}
	else if(fr2 == 10 && DC2 == 20)
	{
		tmp2 /= 10;	
		mood2 = 1;
	}
	else mood2 = 0;
	high1 = (tmp1 * DC1) / 100;
	low1 = tmp1 - high1;
	highload1 = 65536 - high1 + 40;
	lowload1 = 65536 - low1 + 40;
	
	HighRH1 = (uchar)(highload1 / 256);
	HighRL1 = (uchar)(highload1 % 256);
	LowRH1 = (uchar)(lowload1 / 256);
	LowRL1 = (uchar)(lowload1 % 256);
	
	
	high2 = (tmp2 * DC2) / 100;
	low2 = tmp2 - high2;
	highload2 = 65536 - high2 + 11;
	lowload2 = 65536 - low2 + 11;
	
	HighRH2 = (uchar)(highload2 / 256);
	HighRL2 = (uchar)(highload2 % 256);
	LowRH2 = (uchar)(lowload2 / 256);
	LowRL2 = (uchar)(lowload2 % 256);
	
	TMOD &= 0x00;			//设置定时器模式
	TMOD |= 0x11;			//设置定时器模式
	TH0 = HighRH1;				//设置定时初始值
	TL0 = HighRL1;				//设置定时初始值
	TL1 = HighRL2;				//设置定时初始值
	TH1 = HighRH2;				//设置定时初始值
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
	
	PWMOUT1 = 1;
	PWMOUT2 = 1;
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

void Nixie(unsigned char Location,Number)			//数码管显示
{
	switch(Location)
	{
		case 1:P2_4=1;P2_3=1;P2_2=1;break;
		case 2:P2_4=1;P2_3=1;P2_2=0;break;
		case 3:P2_4=1;P2_3=0;P2_2=1;break;
		case 4:P2_4=1;P2_3=0;P2_2=0;break;
		case 5:P2_4=0;P2_3=1;P2_2=1;break;
		case 6:P2_4=0;P2_3=1;P2_2=0;break;
		case 7:P2_4=0;P2_3=0;P2_2=1;break;
		case 8:P2_4=0;P2_3=0;P2_2=0;break;
	}
	P0=led_code[Number];
	Delay(1); 
	P0=0x00;
}

void main()
{
	EA = 1;
	f1 = 10;
	DR1 = 50;
	f2 = 20;
	DR2 = 60;
	pwm(f1, DR1, f2, DR2);
	QS = f1;															
	HS = DR1;
	while(1)
	{
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
		
		Nixie(8,state2 + 1);
		Nixie(7,state);
		Nixie(6,key+1);
		if(Key5 == 0 && flag5 == 0)								//独立5								
		{
			High5++;
			if(High5 >= 1)				//按下检测>=10ms
			{
				High5 = 0;
				flag5 = 1;
			}
		}
		else if(Key5 != 0 && flag5 == 1)
		{
			Low5++;
			if(Low5 >= 1)				//松开检测>=10ms
			{
				state2++;
				state2 %= 2;
				flag5 = 0;
			}
		}
		if(!state2)
		{
//			pwm(f1, DR1, f2, DR2);
			if(P3_1 == 0 && flag1 == 0)					//独1											
			{
				High1++;
				if(High1 >= 1)
				{
					High1 = 0;
					flag1 = 1;
				}
			}
			else if(P3_1 != 0 && flag1 == 1)
			{
				Low1++;
				if(Low1 >= 1)
				{
					state++;
					if(state == 3) state = 1;
					flag1 = 0;
				}
			}
			switch(state)
			{
				case 1:
				{
					QS = f1;															
					HS = DR1;
					switch(key_multi())													//单双击 长摁
					{
						case 21:														//独2单击
						{
							f1 += 10;
							if(f1 > 50) f1 = 50;
							pwm(f1, DR1, f2, DR2);
						}break;
						case 22:														//独2双击
						{
							f1 -= 10;
							if(f1 < 10) f1 = 10;
							pwm(f1, DR1, f2, DR2);
						}break;
						case 31:														//独3单击
						{
							DR1 += 10;
							if(DR1 > 90) DR1 = 90;
							pwm(f1, DR1, f2, DR2);
						}break;
						case 32:														//独3双击
						{
							DR1 -= 10;
							if(DR1 < 10) DR1 = 10;
							pwm(f1, DR1, f2, DR2);
						}break;
					}
				}break;
				case 2 :
				{
					QS = f2;															
					HS = DR2;
					switch(key_multi())													//单双击 长摁
					{
						case 21:														//独2单击
						{
							f2 += 10;
							if(f2 > 50) f2 = 50;
							pwm(f1, DR1, f2, DR2);
						}break;
						case 22:														//独2双击
						{
							f2 -= 10;
							if(f2 < 10) f2 = 10;
							pwm(f1, DR1, f2, DR2);
						}break;
						case 31:														//独3单击
						{
							DR2 += 10;
							if(DR2 > 90) DR2 = 90;
							pwm(f1, DR1, f2, DR2);
						}break;
						case 32:														//独3双击
						{
							DR2 -= 10;
							if(DR2 < 10) DR2 = 10;
							pwm(f1, DR1, f2, DR2);
						}break;
					}
				}break;
			}
			
			P1=0xFF;
			P1_3=0;
			if(P1_6==0 && flag3 == 0)					//5键存数据											
			{
				High3++;
				if(High3 >= 1)
				{
					High3 = 0;
					flag3 = 1;
				}
			}
			else if(P1_6!=0 && flag3 == 1)
			{
				Low3++;
				if(Low3 >= 1)
				{
					AT24C02_WriteByte(0,f1);
					Delay(5);
					AT24C02_WriteByte(1,DR1);
					Delay(5);
					AT24C02_WriteByte(2,f2);
					Delay(5);
					AT24C02_WriteByte(3,DR2);
					Delay(5);
					flag3 = 0;
				}
			}
			
			P1=0xFF;
			P1_0=0;
			if(P1_6==0 && flag4 == 0)					//8键读数据											
			{
				High4++;
				if(High4 >= 1)
				{
					High4 = 0;
					flag4 = 1;
				}
			}
			else if(P1_6!=0 && flag4 == 1)
			{
				Low4++;
				if(Low4 >= 1)
				{
					f1 = AT24C02_ReadByte(0);
					DR1 = AT24C02_ReadByte(1);
					f2 = AT24C02_ReadByte(2);
					DR2 = AT24C02_ReadByte(3);
					pwm(f1, DR1, f2, DR2);
					flag4 = 0;
				}
			}
		}
		else
		{
			UART_Init();
			Timer0_Init();
			if(mood11 == 3)
			{
				f1 = 1000 / (time14 - time12);
				DR1 = 100 * (time13 - time12) / (time14 - time12);
				time11 = 0;
				mood11 = 0;	
			}
			if(mood12 == 3)
			{
				f2 = 1000 / (time24 - time22);
				DR2 = 100 * (time23 - time22) / (time24 - time22);
				time21 = 0;
				mood12 = 0;	
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
					flag1 = 0;
				}
			}
			if(!key) {QS = f1;HS = DR1;}
			else {QS = f2;HS = DR2;}
			if(counter >= 1000)
			{
				counter = 0;
				UART_SendByte1('c');UART_SendByte1('h');UART_SendByte1('1');UART_SendByte1(':');UART_SendByte1('f');UART_SendByte1('=');
				UART_SendByte2(f1);
				UART_SendByte1(',');UART_SendByte1('d');UART_SendByte1('=');
				UART_SendByte2(DR1);
				UART_SendByte1(';');
				UART_SendByte1('c');UART_SendByte1('h');UART_SendByte1('2');UART_SendByte1(':');UART_SendByte1('f');UART_SendByte1('=');
				UART_SendByte2(f2);
				UART_SendByte1(',');UART_SendByte1('d');UART_SendByte1('=');
				UART_SendByte2(DR2);
				UART_SendByte1(';');
			}
		}
		Delay(10);
	}
}