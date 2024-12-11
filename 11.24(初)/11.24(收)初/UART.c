#include <REGX52.H>
#include "Delay.h"
#include <stdio.h>

/**
  * @brief  串口初始化，4800bps@12.000MHz
  * @param  无
  * @retval 无
  */
void UART_Init()	//4800bps@11.0592MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xF4;			//设置定时初始值
	TH1 = 0xF4;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
//	EA = 1;				//开启总中断
	ES = 1;				//开启串口总中断
//	PT1 = 1;
}


/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
//void UART_SendByte(unsigned char Byte)
//{
//	SBUF=Byte;
//	while(TI==0);
//	TI=0;
//}

void UART_SendByte1(unsigned char Byte)			//发送一字节
{
	SBUF = Byte;
	while(TI == 0);
	TI = 0;
}

void UART_SendByte2(unsigned int number) 		// 发送十进制数
{
    char str[10],i;
	sprintf(str, "%u", number); 		// 将十进制数转换为字符串
    for(i = 0; str[i] != '\0'; i++)
	{
        SBUF = str[i]; 		
        while(TI == 0); 		
        TI = 0; 		
	}
}

//串口中断函数模板
//void UART_Routine() interrupt 4
//{
//	unsigned char str2[10];
//	if(TI==1)
//	{
//		TI=0;
//	}
//		char rchar = SBUF; 
//		RI = 0;
//		if (rchar >= '0' && rchar <= '9') 
//		{
//			str2[p++] = rchar; 
//			if (p >= 10) 
//			{ 
//				p = 0;
//			}
//		} 
//		else 
//		{
//			if (p > 0) 
//			{
//				str2[p] = '\0'; 
//				m = atoi(str2); 
//				p = 0; 
//			}
//		}
//	}
//}

