#include"main.h"
using namespace std;


extern char buf[half_buf_size * 2];
extern char* lexemebegin;
extern char* forwardp;
extern char token[half_buf_size];

int keynum = 32;
char key[32][20] = { "auto", "break", "case", "char", "const", "continue", "default", "do", 
					"double", "else", "enum", "extern", "float", "for", "goto", "if",
					"int", "long", "register", "return", "short", "signed", "sizeof", "static",
					"struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

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
			if (C == EOF)
			{
				ter = true;
				break;
			}
			get_npc(inprog, C);
			if (C >= 'a' && C <= 'z' || C >= 'A' && C <= 'Z'|| C == '_')
				//进入标识符状态
				state = 1;
			else if (C >= '0' && C <= '9')
				//进入数字状态
				state = 2;
			else
				switch (C)
				{
				
				}
			break;
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
				cur--;
				token[cur] = '\0';
				state = 0;
				//判断是否是关键字
				int iskey = reserve(token);
				if (iskey != -1)
					//识别结果：关键字
					add();
				else
					//识别结果：普通标识符
				{
					//将标识符加入符号表
				}
			}
			break;
		case 2://数字状态
			token[cur] = C;
			cur++;
			C = get_char(inprog);
			if (digit(C))
				//还在整数状态
				state = 2;
			else if (C == '.')
				//进入小数点状态
				state = 3;
			else if (C == 'E' || C == 'e')
				//进入指数状态
				state = 5;
			else
				//在整数状态结束
			{
				cur--;
				token[cur] = '\0';
				state = 0;
				//识别结果：整数
				add();
			}
			break;
		case 3://小数点状态
			token[cur] = C;
			cur++;
			C = get_char(inprog);
			if (digit(C))
				state = 4;
			else
			{
				//在小数点状态出现了错误字符，错误处理

				state = 0;
			}
			break;
		case 4://小数状态
			token[cur] = C;
			cur++;
			C = get_char(inprog);
			if (digit(C))
				state = 4;
			else if (C == 'E' || C == 'e')
				state = 5;
			else
			{
				cur--;
				token[cur] = '\0';
				state = 0;
				//识别结果：浮点数
				add();
			}
			break;
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

int reserve(char* token)
{
	for (int i = 0; i < keynum; i++)
		if (strcmp(token, key[i]) == 0)
			return i;

	return -1;
}

void add()
{

}