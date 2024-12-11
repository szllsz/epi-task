#ifndef __KEY_MULTI_H__
#define __KEY_MULTI_H__

#include <REGX52.H>

sbit KEY1 = P3^1; 			//按键IO								//！！！修改此处，可以增加/减少按键数量
sbit KEY2 = P3^0;													//！！！修改此处，可以增加/减少按键数量
sbit KEY3 = P3^2;													//！！！修改此处，可以增加/减少按键数量
sbit KEY4 = P3^3;													//！！！修改此处，可以增加/减少按键数量

#define KEY_STATE_0 0 										//按键状态
#define KEY_STATE_1 1
#define KEY_STATE_2 2
#define KEY_STATE_3 3
#define KEY_STATE_4 4

#define LONG_KEY_TIME 80 									//判定长按时间：LONG_KEY_TIME*10MS = 800MS
#define SINGLE_KEY_TIME 1 								//判定单击时间：SINGLE_KEY_TIME*10MS = 10MS
#define DOUBLE_KEY_TIME 50 								//判定单击时间：DOUBLE_KEY_TIME*10MS = 500MS

#define None_click 0 											//无按键  	返回值：0
#define Single_click 1 										//单击		返回值：1	
#define Double_click 2										//双击		返回值：2
#define Long_click 3 											//长按		返回值：3

#define uchar unsigned char
#define uint unsigned int	
	
#define Key1_Single_Click 11							//！！！修改此处，可以增加/减少按键数量
#define Key1_Double_Click 12							//！！！修改此处，可以增加/减少按键数量
#define Key1_Long_Click 13								//！！！修改此处，可以增加/减少按键数量

#define Key2_Single_Click 21							//！！！修改此处，可以增加/减少按键数量
#define Key2_Double_Click 22							//！！！修改此处，可以增加/减少按键数量
#define Key2_Long_Click 23								//！！！修改此处，可以增加/减少按键数量

#define Key3_Single_Click 31							//！！！修改此处，可以增加/减少按键数量
#define Key3_Double_Click 32							//！！！修改此处，可以增加/减少按键数量
#define Key3_Long_Click 33								//！！！修改此处，可以增加/减少按键数量

#define Key4_Single_Click 41							//！！！修改此处，可以增加/减少按键数量
#define Key4_Double_Click 42							//！！！修改此处，可以增加/减少按键数量
#define Key4_Long_Click 43								//！！！修改此处，可以增加/减少按键数量

unsigned char key_multi(void);						//多功能按键扫描函数，返回 键值+键功能 复合值
unsigned char keyscan();									//普通按键扫描函数，返回 键值
#endif