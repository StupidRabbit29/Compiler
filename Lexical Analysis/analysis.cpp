#include"main.h"
using namespace std;


extern char buf[half_buf_size * 2];
extern char* lexemebegin;
extern char* forwardp;
extern char token[half_buf_size];

Status analysis(ifstream& inprog)
{
	//初始化缓存区
	memset(buf, 0, sizeof(buf));
	buf[half_buf_size - 1] = buf[half_buf_size * 2 - 1] = -1;
	memset(token, 0, sizeof(token));
	lexemebegin = buf;
	forwardp = buf;
	int cur = 0;
	getinput(inprog, buf_left);

	int state = 0;
	bool ter = false;
	char C;

	while (!ter)
	{
		switch (state)
		{
		case 0://初始状态
			memset(token, 0, sizeof(token));
			cur = 0;
			//读取一个字符
			C = get_char(inprog);
			get_npc(inprog, C);
			if (C >= 'a' && C <= 'z' || C >= 'A' && C <= 'Z')
			{
				//进入标识符状态
				state = 1;
				break;
			}
			else if (C >= '0' && C <= '9')
			{
				//进入数字状态
				state = 2;
				break;
			}
			else
				switch (C)
				{
				
				}
		case 1://标识符状态
			token[cur] = C;
			cur++;
			C = get_char(inprog);
			//判断是否是合格的字符，属于超前验证
			if (letter(C) || digit(C) || C == '_')
				state = 1;
			else
				//已达到标识符结尾
			{

			}
		case 2://数字状态
		case 3://小数点状态
		case 4://小数状态
		case 5://指数状态
		case 6://
		case 7://
		case 8://
		case 9://
		case 10://
		case 11://
		case 12://
		case 13://
		default:
		}



		

		
		

		if (ter)
			//编译程序完成，完成收尾工作
		{

			break;
		}


	}

	return OK;
}

//从缓存区读取一个字符，前向指针向前移动一个单位
char get_char(ifstream& inprog)
{
	char C = *forwardp;
	if (C == EOF)
		//判断是否到达文件尾或到达双缓存区的两个边界
	{
		if (forwardp == buf + half_buf_size)
			getinput(inprog, buf_right);
		else if (forwardp == buf)
			getinput(inprog, buf_left);
		else
			return C;

		forwardp = buf + (forwardp + 1 - buf) % (half_buf_size * 2);
		C = get_char(inprog);
	}
	else
		forwardp = buf + (forwardp + 1 - buf) % (half_buf_size * 2);

	return C;
}

void get_npc(ifstream& inprog, char& C)
{
	if (C == ' ' || C == '\t' || C == '\n')
	{
		C = get_char(inprog);
		get_npc(inprog, C);
	}
}

bool letter(char& C)
{
	if (C >= 'A' && C <= 'Z' || C >= 'a' && C <= 'z')
		return true;
	else
		return false;
}

bool digit(char& C)
{
	if (C >= '0' && C <= '9')
		return true;
	else
		return false;
}
