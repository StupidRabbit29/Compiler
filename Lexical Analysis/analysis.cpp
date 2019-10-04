#include"main.h"
using namespace std;

vector<long long>INT;
vector<double>FLOAT;
vector<string>Word;
vector<Token>TOKEN;

int line = 1;

extern char buf[half_buf_size * 2];
extern char* lexemebegin;
extern char* forwardp;
extern char token[half_buf_size];

const char* key[keynum] = 
{ 
	"auto", "break", "case", "char", "const", "continue", "default", "do", 
	"double", "else", "enum", "extern", "float", "for", "goto", "if",
	"int", "long", "register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

const char* opt[optnum] =
{
	"+", "+=", "-", "-=", "*", "*=", "/", "/=", "%", "%=", "^", "^=", 
	"#", "&", "\\", "?"
};

const char* end[endnum] =
{
	"(", ")", "{", "}", "[", "]", ":", ";", ",", ".", "\"", "\'"
};

const char* other[othernum] =
{
	"!", "&&", "||", "<", "<=", ">", ">=", "=", "==", "!="
};

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
				case '#':state = 0; add(OPTION, 12); break;
				case '\\':state = 0; add(OPTION, 14); break;
				case '?':state = 0; add(OPTION, 15); break;
				case '(':state = 0; add(END, 0); break;
				case ')':state = 0; add(END, 1); break;
				case '{':state = 0; add(END, 2); break;
				case '}':state = 0; add(END, 3); break;
				case '[':state = 0; add(END, 4); break;
				case ']':state = 0; add(END, 5); break;
				case ':':state = 0; add(END, 6); break;
				case ';':state = 0; add(END, 7); break;
				case ',':state = 0; add(END, 8); break;
				case '.':state = 0; add(END, 9); break;
				case '\"':state = 0; add(END, 10); break;
				case '\'':state = 0; add(END, 11); break;
				}
			break;
		case 1://标识符状态
			cat(C, cur);
			C = get_char(inprog);
			//判断是否是合格的字符，属于超前验证
			if (letter(C) || digit(C) || C == '_')
				state = 1;
			else
				//已达到标识符结尾
			{
				retract();
				state = 0;
				//判断是否是关键字
				int iskey = reserve(token);
				if (iskey != -1)
					//识别结果：关键字
					add(KEYWORD, iskey);
				else
					//识别结果：普通标识符
					add(WORD, 0);
			}
			break;
		case 2://数字状态
			cat(C, cur);
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
				retract();
				state = 0;
				//识别结果：整数
				add(INTDIG, 0);
			}
			break;
		case 3://小数点状态
			cat(C, cur);
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
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 4;
			else if (C == 'E' || C == 'e')
				state = 5;
			else
			{
				retract();
				state = 0;
				//识别结果：浮点数
				add(FLOATDIG, 0);
			}
			break;
		case 5://指数状态
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 7;
			else if (C == '-' || C == '+')
				state = 6;
			else
			{
				retract();
				//在指数状态出现错误字符，错误处理

				state = 0;
			}
			break;
		case 6://指数的符号后的第一个数字
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 7;
			else
			{
				retract();
				//指数部分符号后没有数字，错误处理

				state = 0;
			}
			break;
		case 7://指数的数字部分
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 7;
			else
			{
				retract();
				state = 0;
				//识别结果，带有指数部分的浮点数
				add(FLOATDIG, 0);
			}
			break;
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

	if (C == '\n')
		line++;

	return C;
}

//判断读入的字符是否是空字符
void get_npc(ifstream& inprog, char& C)
{
	if (C == ' ' || C == '\t' || C == '\n')
	{
		C = get_char(inprog);
		get_npc(inprog, C);
	}
}

//判断字符是否为字母
bool letter(char& C)
{
	if (C >= 'A' && C <= 'Z' || C >= 'a' && C <= 'z')
		return true;
	else
		return false;
}

//判断字符是否为数字
bool digit(char& C)
{
	if (C >= '0' && C <= '9')
		return true;
	else
		return false;
}

//将字符C加入到记号缓存区中
void cat(char& C, int& cur)
{
	token[cur] = C;
	cur++;
}

//超前识别截止，前向指针后退，经过分析，前向指针后退时，不会碰到EOF，不会越界
void retract(void)
{
	forwardp--;
}

//判断标识符是否为保留关键字
int reserve(char* token)
{
	for (int i = 0; i < keynum; i++)
		if (strcmp(token, key[i]) == 0)
			return i;

	return -1;
}

//将整数从字符串转换为数字
long long get_int(void)
{
	char* ptr = token;
	long long intiger = 0LL;
	while (*ptr != '\0')
	{
		intiger = intiger * 10 + *ptr - '0';
		ptr++;
	}
	return intiger;
}

//将识别出的记号添加到记号序列中
void add(int type, int num)
{
	string word;
	long long intiger;
	int index;
	Token temp;
	switch (type)
	{
	case 1:
		word = token;
		index = Word.size();
		Word.push_back(word);
		temp.type = WORD;
		temp.index = index;
		temp.line = line;
		TOKEN.push_back(temp);
	case 2:
		temp.type = KEYWORD;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
	case 3:
		intiger = get_int();
		index = INT.size();
		INT.push_back(intiger);
		temp.type = INTDIG;
		temp.index = index;
		temp.line = line;
		TOKEN.push_back(temp);
	case 4:
	case 5:
	case 6:
	case 7:
	default:
	}
}

