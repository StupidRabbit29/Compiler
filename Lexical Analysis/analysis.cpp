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
	"#", "&", "?"
};

const char end[endnum] =
{
	'(', ')', '{', '}', '[', ']', ':', ';', ',', '.', '\"', '\'', '\\'
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
				case '?':state = 0; add(OPTION, 14); break;
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
				case '\\':state = 0; add(END, 12); break;
				case '+':state = 8; break;
				case '-':state = 9; break;
				case '*':state = 10; break;
				case '/':state = 11; break;
				case '%':state = 12; break;
				case '^':state = 13; break;
				case '&':state = 14; break;
				case '<':state = 15; break;
				case '>':state = 16; break;
				case '!':state = 17; break;
				case '=':state = 18; break;
				case '|':state = 19; break;
				default:state = 20; break;
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
				error(Wrong_dot);
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
				error(Wrong_E);
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
				error(Wrong_neg);
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
		case 8://	+状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：+=
				add(OPTION, 1);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：+
				add(OPTION, 0);
			}
			break;
		case 9://	-状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：-=
				add(OPTION, 3);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：-
				add(OPTION, 2);
			}
			break;
		case 10://	*状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：*=
				add(OPTION, 5);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：*
				add(OPTION, 4);
			}
			break;
		case 11://	/状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：/=
				add(OPTION, 7);
			}
			else if (C == '/')
				//注释状态
			{
				while (C != '\n')
					C = get_char(inprog);
			}
			else if (C == '*')
				//注释状态
			{
				C = get_char(inprog);
				while (true)
				{
					while (C != '*')
						C = get_char(inprog);
					C = get_char(inprog);
					if (C == '/')
						break;
				}
			}
			else
			{
				retract();
				state = 0;
				//识别结果：/
				add(OPTION, 6);
			}
			break;
		case 12://	%状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：%=
				add(OPTION, 9);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：%
				add(OPTION, 8);
			}
			break;
		case 13://	^状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：^=
				add(OPTION, 11);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：^
				add(OPTION, 10);
			}
			break;
		case 14://	&状态
			C = get_char(inprog);
			if (C == '&')
			{
				state = 0;
				//识别结果：&&
				add(OTHER, 1);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：&
				add(OPTION, 13);
			}
			break;
		case 15://	<状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：<=
				add(OTHER, 4);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：<
				add(OTHER, 3);
			}
			break;
		case 16://	>状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：>=
				add(OTHER, 6);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：>
				add(OTHER, 5);
			}
			break;
		case 17://	!状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：!=
				add(OTHER, 9);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：!
				add(OTHER, 0);
			}
			break;
		case 18://	=状态
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//识别结果：==
				add(OTHER, 8);
			}
			else
			{
				retract();
				state = 0;
				//识别结果：=
				add(OTHER, 7);
			}
			break;
		case 19://	|状态
			C = get_char(inprog);
			if (C == '|')
			{
				state = 0;
				//识别结果：||
				add(OTHER, 2);
			}
			else
			{
				retract();
				//错误处理：仅识别到一个|
				error(Single_or);
			}
		case 20://	错误处理：识别到非法字符
			error(Invalid_character);
			break;
		default:
			error(Other_wrong);
			break;
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
	double floatnum;
	int index;
	Token temp;

	switch (type)
	{
	case WORD:
		//普通标识符
		word = token;
		index = Word.size();
		Word.push_back(word);
		temp.type = WORD;
		temp.index = index;
		temp.line = line;
		TOKEN.push_back(temp);
	case KEYWORD:
		//关键字
		temp.type = KEYWORD;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
	case INTDIG:
		//整数
		intiger = get_int();
		index = INT.size();
		INT.push_back(intiger);
		temp.type = INTDIG;
		temp.index = index;
		temp.line = line;
		TOKEN.push_back(temp);
	case FLOATDIG:
		//浮点数
	case OPTION:
		//操作符号
		temp.type = OPTION;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
	case END:
		//界符
		temp.type = END;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
	case OTHER:
		//其他符号
		temp.type = OTHER;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
	default:
		error(Other_wrong);
		break;
	}
}

void error(ErrorType type)
{
	switch (type)
	{
	case Wrong_dot:
		cout << "****************************************************************************" << endl
			<< "错误：浮点数的小数点后应当有数字" << endl
			<< "行号：" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Wrong_neg:
		cout << "****************************************************************************" << endl
			<< "错误：浮点数指数部分的+或-号后应当有数字" << endl
			<< "行号：" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Wrong_E:
		cout << "****************************************************************************" << endl
			<< "错误：浮点数指数部分E或e后只能出现（+/-）数字" << endl
			<< "行号：" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Single_or:
		cout << "****************************************************************************" << endl
			<< "错误：用户输入的||仅有一个|" << endl
			<< "行号：" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Invalid_character:
		cout << "****************************************************************************" << endl
			<< "错误：出现C语言不支持的字符" << endl
			<< "行号：" << line << endl
			<< "****************************************************************************" << endl;
		break;
	default:
		cout << "****************************************************************************" << endl
			<< "错误：编译器运行时错误" << endl
			<< "行号：" << line << endl
			<< "****************************************************************************" << endl;
		break;
	}
}