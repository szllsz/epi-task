#include <REGX52.H>
#include "Delay.h"
#include "key_multi.h"
#include "UART.h"
#include "AT24C02.h"
#include <stdio.h>
#include <stdlib.h>

#define uchar unsigned char
#define uint unsigned int
#define length 4

sbit PWMOUT = P2^0;
sbit PWMOUT2 = P2^7;

uchar f,DR,i,state,Loc,Num,t,p,count1,count2 = 20;									//f��Ƶ��  DR��ռ�ձ�
uchar flag1,flag2,flag3,flag4,flag5,High1,High2,High3,High4,High5,Low1,Low2,Low3,Low4,Low5;
uchar display1[length],display2[length],str[10];
uchar HighRH,HighRL,LowRH,LowRL;
uint QS,HS,temp1,temp2,count;

uchar code led_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Timer0_Isr(void) interrupt 1
{
	if(PWMOUT == 1)
	{
		TH0 = HighRH;				
		TL0 = HighRL;
		count1++;
		if(count1 == 100)
		{
			count1 = 0;
			TH0 = LowRH;				
			TL0 = LowRL;
			PWMOUT = 0;
		}
	}
	else
	{
		TH0 = LowRH;				
		TL0 = LowRL;
		count1++;
		if(count1 == 100)
		{
			count1 = 0;
			TH0 = HighRH;				
			TL0 = HighRL;
			PWMOUT = 1;
		}
	}
	
	if(PWMOUT2 == 1)
	{
		TH0 = HighRH;				
		TL0 = HighRL;
		count2++;
		if(count2 == 100)
		{
			count2 = 0;
			TH0 = LowRH;				
			TL0 = LowRL;
			PWMOUT2 = 0;
		}
	}
	else
	{
		TH0 = LowRH;				
		TL0 = LowRL;
		count2++;
		if(count2 == 100)
		{
			count2 = 0;
			TH0 = HighRH;				
			TL0 = HighRL;
			PWMOUT2 = 1;
		}
	}
}

void pwm(uchar fr, DC)
{
	uint high, low, highload, lowload;
	unsigned long tmp;
	
	tmp = (921600) / fr;				//���� 1105920 / 12
	tmp /= 100;							//�����ڷֳ�һ�ٷ� ���������λ��
	high = (tmp * DC) / 100;
	low = tmp - high;
	highload = 65536 - high + 11;
	lowload = 65536 - low + 11;
	
	HighRH = (uchar)(highload / 256);
	HighRL = (uchar)(highload % 256);
	LowRH = (uchar)(lowload / 256);
	LowRL = (uchar)(lowload % 256);
	
	
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ
	TH0 = HighRH;				//���ö�ʱ��ʼֵ
	TL0 = HighRL;				//���ö�ʱ��ʼֵ
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
	EA = 1;					//�������ж�
	
	PWMOUT = 1;

	PWMOUT2 = 1;
}

void Nixie(unsigned char Location,Number)			//�������ʾ
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

void UART_Routine() interrupt 4
{
	if(RI == 1)
	{
		char a = SBUF;
		if(a == 'f') state = 1;
		else if(a == 'd') state = 2;
		RI = 0;
		switch(state)
		{
			case 1:
			{
				char rchar1 = SBUF; 
				if (rchar1 >= '0' && rchar1 <= '9') 
				{
					str[p++] = rchar1; 
					if (p >= 10) 
					{ 
						p = 0;
					}
				} 
				else 
				{
					if (p > 0) 
					{
						str[p] = '\0'; 
						f = atoi(str); 
						p = 0; 
					}
				}
			}break;
			case 2:
			{
				char rchar2 = SBUF; 
				if (rchar2 >= '0' && rchar2 <= '9') 
				{
					str[p++] = rchar2; 
					if (p >= 10) 
					{ 
						p = 0;
					}
				} 
				else 
				{
					if (p > 0) 
					{
						str[p] = '\0'; 
						DR = atoi(str); 
						p = 0; 
					}
				}
			}break;
		}
	}
}

void main()
{
	f = 10;
	DR = 50;
	pwm(f, DR);
	QS = f;															
	HS = DR;
	UART_Init();
	while(1)
	{
		temp1 = QS;
		temp2 = HS;
		for(i = 0; i < length; i++)									//ǰ��λ��ʾ
		{
			display1[length - i - 1] = temp1 % 10;
			temp1 /= 10;
			Nixie(i + 1, display1[i]);
		}
		
		for(i = 0; i < length; i++)									//����λ��ʾ
		{
			display2[length - i - 1] = temp2 % 10;
			temp2 /= 10;
			Nixie(i + 5, display2[i]);
		}
		if(QS == 1111)		//ģʽһ
		{
			HS = f;
			
			P1=0xFF;
			P1_3=0;
			if(P1_7==0 && flag1 == 0)					//1��											//1��
			{
				High1++;
				if(High1 >= 1)
				{
					High1 = 0;
					flag1 = 1;
				}
			}
			else if(P1_7!=0 && flag1 == 1)
			{
				Low1++;
				if(Low1 >= 1)
				{
					if(f > 40) f -= 40 ;
					else f += 10;
					pwm(f, DR);
					flag1 = 0;
				}
			}
			
			P1=0xFF;
			P1_0=0;
			if(P1_7==0 && flag2 == 0)							//4��									//1��
			{
				High2++;
				if(High2 >= 1)
				{
					High2 = 0;
					flag2 = 1;
				}
			}
			else if(P1_7!=0 && flag2 == 1)
			{
				Low2++;
				if(Low2 >= 1)
				{
					if(f == 50) f -= 49;
					else f += 1;
					pwm(f, DR);
					flag2 = 0;
				}
			}
		}
		else if(QS == 2222)
		{
			HS = DR;
			
			P1=0xFF;
			P1_3=0;
			if(P1_7==0 && flag1 == 0)																//1��
			{
				High1++;
				if(High1 >= 1)
				{
					High1 = 0;
					flag1 = 1;
				}
			}
			else if(P1_7!=0 && flag1 == 1)
			{
				Low1++;
				if(Low1 >= 1)
				{
					if(DR > 90) DR = 100;
					else DR += 10;
					pwm(f, DR);
					flag1 = 0;
				}
			}
			P1=0xFF;
			P1_0=0;
			if(P1_7==0 && flag2 == 0)																//1��
			{
				High2++;
				if(High2 >= 1)
				{
					High2 = 0;
					flag2 = 1;
				}
			}
			else if(P1_7!=0 && flag2 == 1)
			{
				Low2++;
				if(Low2 >= 1)
				{
					if(DR < 10 ) DR = 0;
					else DR -= 10;
					pwm(f, DR);
					flag2 = 0;
				}
			}
		}
		else
		{
			QS = f;															
			HS = DR;
		}
		
		switch(key_multi())													//��˫�� ����
		{
			case 41:														//����
			{
				QS = 1111;
				HS = f;
			}break;
			case 42:														//˫��
			{
				QS = 2222;
				HS = DR;
			}break;
			case 43:														//����
			{
				QS = f;
				HS = DR;
			}break;
		}
		
		P1=0xFF;
		P1_3=0;
		if(P1_6==0 && flag3 == 0)					//5��������											//1��
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
				AT24C02_WriteByte(0,f);
				Delay(5);
				AT24C02_WriteByte(1,DR);
				Delay(5);
				flag3 = 0;
			}
		}
		P1=0xFF;
		P1_0=0;
		if(P1_6==0 && flag4 == 0)					//8��������											//1��
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
				f = AT24C02_ReadByte(0);
				DR = AT24C02_ReadByte(1);
				pwm(f, DR);
				flag4 = 0;
			}
		}
		
		
		
		P1=0xFF;
		P1_0=0;
		if(P1_4==0 && flag5 == 0)							//16�����ʹ���											//1��
		{
			High5++;
			if(High5 >= 1)
			{
				High5 = 0;
				flag5 = 1;
			}
		}
		else if(P1_4!=0 && flag5 == 1)
		{
			Low5++;
			if(Low5 >= 1)
			{
				UART_SendByte1('f');UART_SendByte1('=');UART_SendByte2(f);
				UART_SendByte1(',');
				UART_SendByte1('d');UART_SendByte1('=');UART_SendByte2(DR);
				flag5 = 0;
			}
		}
		Delay(10);
	}

}