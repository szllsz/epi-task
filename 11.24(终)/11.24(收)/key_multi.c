#include <key_multi.h>

unsigned char keyscan()
{
	unsigned char keynumber;
	if(!KEY1) keynumber = 1;								//！！！修改此处，可以增加/减少按键数量
	else if(!KEY2) keynumber = 2;							//！！！修改此处，可以增加/减少按键数量
	else if(!KEY3) keynumber = 3;							//！！！修改此处，可以增加/减少按键数量
	else if(!KEY4) keynumber = 4;							//！！！修改此处，可以增加/减少按键数量
	else keynumber = 0;
	return keynumber;
}

unsigned char key_multi(void)
{
	static uchar key_state; 									//按键状态变量
	static uint key_time; 										//按键计时变量
	static uchar key_press_nums;							//按键次数变量		
	static uint key_time_double;							//按键双击时间
	static uchar keynum;											//键值
	static uchar new_keynum;									//新的键值
	uchar key_press, key_return;							//键值和返回值
	
	key_return = 0; 									//清除返回按键值
	
	switch(keyscan())
	{
		case 1:key_press = KEY1;new_keynum = 1;break;		
		case 2:key_press = KEY2;new_keynum = 2;break;		
		case 3:key_press = KEY3;new_keynum = 3;break;		
		case 4:key_press = KEY4;new_keynum = 4;break;		
		default:key_press = 1;break;
	}
	if(key_press_nums > 0) 										//按键次数大于0，说明有按键
	{
		key_time_double++;											//双击计时
		if(key_time_double >= DOUBLE_KEY_TIME && key_time_double < LONG_KEY_TIME) key_state = KEY_STATE_2;//在长按时间和双击时间设定范围内，到状态2
	}
	
	switch (key_state)
	{
		case KEY_STATE_0: 											//按键状态0：判断有无按键按下
		{
			if (!key_press) 											//有按键按下
			{
				key_time = 0; 											//清零时间间隔计数
				key_state = KEY_STATE_1; 						//然后进入 按键状态1
			}
		}break;
		case KEY_STATE_1: 											//按键状态1：软件消抖(确定按键是否有效，而不是误触)。按键有效的定义：按键持续按下超过设定的消抖时间。
		{
			if (!key_press)
			{
				key_time++; 												//一次10ms
				if(key_time>=SINGLE_KEY_TIME) 			//消抖时间为：SINGLE_KEY_TIME*10ms = 10ms;
				{
					key_state = KEY_STATE_2; 					//如果按键时间超过 消抖时间，即判定为按下的按键有效。进入按键状态2，继续判定到底是单击、双击还是长按
					key_press_nums++;									//按键次数+1
				}
			}
			else key_state = KEY_STATE_0; 				//如果按键时间没有超过，判定为误触，按键无效，返回 按键状态0，继续等待按键
		}break;
		case KEY_STATE_2: 											//按键状态2：判定按键有效的种类：是单击，双击，还是长按
		{
			if(key_press) 												//如果按键在 设定的双击时间内释放，则判定为单击/双击
			{
				if(key_time_double < 50 ) key_state = KEY_STATE_0;							//双击时间达到前，再次扫描按键
				if(key_time_double >= 50 && key_time_double < 80) 	//达到双击设定时间后
				{
					if(key_press_nums == 1) 
						key_return = Single_click + new_keynum*10; 				//返回有效按键值：单击
					else if(key_press_nums ==2 && new_keynum == keynum) 
						key_return = Double_click + new_keynum*10; 	//返回有效按键值：双击
					key_state = KEY_STATE_0; 					//返回 按键状态0，继续等待按键
					key_press_nums = 0;								//清空按键次数
					key_time_double = 0;							//清空双击计时
				}
			}
			else
			{	
				key_time++;													//按键一直按着的话，长按计时
				if(key_time >= LONG_KEY_TIME) 			//如果按键时间超过 设定的长按时间(LONG_KEY_TIME*10ms=80*10ms=800ms), 则判定为 长按
				{
					key_return = Long_click + new_keynum*10; 	//返回有效键值值：长按
					key_state = KEY_STATE_3; 					//去状态3，等待按键释放
				}
			}
		}break;
		case KEY_STATE_3: 											//等待按键释放
		{
			if (key_press)
			{
				key_state = KEY_STATE_0; 						//按键释放后，进入 按键状态0 ，进行下一次按键的判定
				key_press_nums = 0;									//清空按键次数
				key_time_double = 0;								//清空双击计时
			}
		}break;
		default:key_state = KEY_STATE_0;break;	//特殊情况：key_state是其他值得情况，清零key_state。这种情况一般出现在 没有初始化key_state，第一次执行这个函数的时候
	}
	keynum = new_keynum;											//将新获取的键值赋值给键值
	return key_return; 												//返回 按键值
}