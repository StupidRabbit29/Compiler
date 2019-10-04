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
	//��ʼ��������
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
		case 0://��ʼ״̬
			memset(token, 0, sizeof(token));
			cur = 0;
			//��ȡһ���ַ�
			C = get_char(inprog);
			if (C == EOF)
			{
				ter = true;
				break;
			}
			get_npc(inprog, C);
			if (C >= 'a' && C <= 'z' || C >= 'A' && C <= 'Z'|| C == '_')
				//�����ʶ��״̬
				state = 1;
			else if (C >= '0' && C <= '9')
				//��������״̬
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
		case 1://��ʶ��״̬
			cat(C, cur);
			C = get_char(inprog);
			//�ж��Ƿ��Ǻϸ���ַ������ڳ�ǰ��֤
			if (letter(C) || digit(C) || C == '_')
				state = 1;
			else
				//�Ѵﵽ��ʶ����β
			{
				retract();
				state = 0;
				//�ж��Ƿ��ǹؼ���
				int iskey = reserve(token);
				if (iskey != -1)
					//ʶ�������ؼ���
					add(KEYWORD, iskey);
				else
					//ʶ��������ͨ��ʶ��
					add(WORD, 0);
			}
			break;
		case 2://����״̬
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				//��������״̬
				state = 2;
			else if (C == '.')
				//����С����״̬
				state = 3;
			else if (C == 'E' || C == 'e')
				//����ָ��״̬
				state = 5;
			else
				//������״̬����
			{
				retract();
				state = 0;
				//ʶ����������
				add(INTDIG, 0);
			}
			break;
		case 3://С����״̬
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 4;
			else
			{
				//��С����״̬�����˴����ַ���������

				state = 0;
			}
			break;
		case 4://С��״̬
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
				//ʶ������������
				add(FLOATDIG, 0);
			}
			break;
		case 5://ָ��״̬
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 7;
			else if (C == '-' || C == '+')
				state = 6;
			else
			{
				retract();
				//��ָ��״̬���ִ����ַ���������

				state = 0;
			}
			break;
		case 6://ָ���ķ��ź�ĵ�һ������
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 7;
			else
			{
				retract();
				//ָ�����ַ��ź�û�����֣�������

				state = 0;
			}
			break;
		case 7://ָ�������ֲ���
			cat(C, cur);
			C = get_char(inprog);
			if (digit(C))
				state = 7;
			else
			{
				retract();
				state = 0;
				//ʶ����������ָ�����ֵĸ�����
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
			//���������ɣ������β����
		{

			break;
		}


	}

	return OK;
}

//�ӻ�������ȡһ���ַ���ǰ��ָ����ǰ�ƶ�һ����λ
char get_char(ifstream& inprog)
{
	char C = *forwardp;
	if (C == EOF)
		//�ж��Ƿ񵽴��ļ�β�򵽴�˫�������������߽�
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

//�ж϶�����ַ��Ƿ��ǿ��ַ�
void get_npc(ifstream& inprog, char& C)
{
	if (C == ' ' || C == '\t' || C == '\n')
	{
		C = get_char(inprog);
		get_npc(inprog, C);
	}
}

//�ж��ַ��Ƿ�Ϊ��ĸ
bool letter(char& C)
{
	if (C >= 'A' && C <= 'Z' || C >= 'a' && C <= 'z')
		return true;
	else
		return false;
}

//�ж��ַ��Ƿ�Ϊ����
bool digit(char& C)
{
	if (C >= '0' && C <= '9')
		return true;
	else
		return false;
}

//���ַ�C���뵽�ǺŻ�������
void cat(char& C, int& cur)
{
	token[cur] = C;
	cur++;
}

//��ǰʶ���ֹ��ǰ��ָ����ˣ�����������ǰ��ָ�����ʱ����������EOF������Խ��
void retract(void)
{
	forwardp--;
}

//�жϱ�ʶ���Ƿ�Ϊ�����ؼ���
int reserve(char* token)
{
	for (int i = 0; i < keynum; i++)
		if (strcmp(token, key[i]) == 0)
			return i;

	return -1;
}

//���������ַ���ת��Ϊ����
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

//��ʶ����ļǺ���ӵ��Ǻ�������
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

