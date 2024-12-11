#include <REGX52.H>

sbit key = P3^1;

unsigned char flag, High, Low;

void Key()
{
	if(key == 0 && flag == 0)															
	{
		High++;
		if(High >= 1)				//按下检测>=10ms
		{
			High = 0;
			flag = 1;
		}
	}
	else if(key != 0 && flag == 1)
	{
		Low++;
		if(Low >= 1)				//松开检测>=10ms
		{
			//执行程序部分
			
			flag = 0;
		}
	}
}