#include"main.h"
using namespace std;

vector<long long>INT;		//������
vector<double>FLOAT;		//��������
vector<string>Word;			//��ʶ����
vector<Token>TOKEN;			//�Ǻ�����

int line = 1;

extern char buf[half_buf_size * 2];
extern char* lexemebegin;
extern char* forwardp;
extern char token[half_buf_size];

extern int totalnum;

//�ؼ���
const char* key[keynum] = 
{ 
	"auto", "break", "case", "char", "const", "continue", "default", "do", 
	"double", "else", "enum", "extern", "float", "for", "goto", "if",
	"int", "long", "register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};
//������
const char* opt[optnum] =
{
	"+", "+=", "-", "-=", "*", "*=", "/", "/=", "%", "%=", "^", "^=", 
	"#", "&", "?", "|"
};
//���
const char End[endnum] =
{
	'(', ')', '{', '}', '[', ']', ':', ';', ',', '.', '\"', '\'', '\\'
};
//��������
const char* other[othernum] =
{
	"!", "&&", "||", "<", "<=", ">", ">=", "=", "==", "!=", "_", "~"
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
			get_npc(inprog, C);
			if (C == EOF) { ter = true; break; }
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
				case '_':state = 0; add(OTHER, 10); break;
				case '~':state = 0; add(OTHER, 11); break;
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
			else if (letter(C))
				//������
			{
				//�����������ı�ʶ��
				while (letter(C) || digit(C) || C == '_')
					C = get_char(inprog);

				retract();
				state = 0;
				error(Wrong_identifier);
			}
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
				//��������λ��
				while (!(C == '\t' || C == '\n' || C == ' '))
					C = get_char(inprog);

				retract();
				//��С����״̬�����˴����ַ���������
				error(Wrong_dot);
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
				//��������λ��
				while (!(C == '\t' || C == '\n' || C == ' '))
					C = get_char(inprog);

				retract();
				//��ָ��״̬���ִ����ַ���������
				error(Wrong_E);
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
				//��������λ��
				while (!(C == '\t' || C == '\n' || C == ' '))
					C = get_char(inprog);

				retract();
				//ָ�����ַ��ź�û�����֣�������
				error(Wrong_neg);
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
		case 8://	+״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������+=
				add(OPTION, 1);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������+
				add(OPTION, 0);
			}
			break;
		case 9://	-״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������-=
				add(OPTION, 3);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������-
				add(OPTION, 2);
			}
			break;
		case 10://	*״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������*=
				add(OPTION, 5);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������*
				add(OPTION, 4);
			}
			break;
		case 11://	/״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������/=
				add(OPTION, 7);
			}
			else if (C == '/')
				//ע��״̬
			{
				while (C != '\n')
					C = get_char(inprog);
				state = 0;
			}
			else if (C == '*')
				//ע��״̬
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
				state = 0;
			}
			else
			{
				retract();
				state = 0;
				//ʶ������/
				add(OPTION, 6);
			}
			break;
		case 12://	%״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������%=
				add(OPTION, 9);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������%
				add(OPTION, 8);
			}
			break;
		case 13://	^״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������^=
				add(OPTION, 11);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������^
				add(OPTION, 10);
			}
			break;
		case 14://	&״̬
			C = get_char(inprog);
			if (C == '&')
			{
				state = 0;
				//ʶ������&&
				add(OTHER, 1);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������&
				add(OPTION, 13);
			}
			break;
		case 15://	<״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������<=
				add(OTHER, 4);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������<
				add(OTHER, 3);
			}
			break;
		case 16://	>״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������>=
				add(OTHER, 6);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������>
				add(OTHER, 5);
			}
			break;
		case 17://	!״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������!=
				add(OTHER, 9);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������!
				add(OTHER, 0);
			}
			break;
		case 18://	=״̬
			C = get_char(inprog);
			if (C == '=')
			{
				state = 0;
				//ʶ������==
				add(OTHER, 8);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������=
				add(OTHER, 7);
			}
			break;
		case 19://	|״̬
			C = get_char(inprog);
			if (C == '|')
			{
				state = 0;
				//ʶ������||
				add(OTHER, 2);
			}
			else
			{
				retract();
				state = 0;
				//ʶ������|
				add(OPTION, 15);
			}
			break;
		case 20://	������ʶ�𵽷Ƿ��ַ�
			error(Invalid_character);
			state = 0;
			break;
		default:
			if (C == EOF) { ter = true; break; }
			error(Other_wrong);
			state = 0;
			break;
		}

		if (C == EOF)
			ter = true;

		if (ter)
			//���������ɣ������β����
		{
			cout << "****************************************************************************" << endl
				<< "\tLexical Analysis is successfully completed!" << endl
				<< "\tIn your program, we get:" << endl
				<< "\tLines:" << line - 1 << endl
				<< "\t�ַ�����:\t" << totalnum << endl
				<< "\t��ʶ��:\t" << Word.size() << endl
				<< "\t����:\t" << INT.size() << endl
				<< "\t������:\t" << FLOAT.size() << endl
				<< "****************************************************************************" << endl;
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
		forwardp = buf + (forwardp + 1 - buf) % (half_buf_size * 2);

		if (forwardp == buf + half_buf_size)
			getinput(inprog, buf_right);
		else if (forwardp == buf)
			getinput(inprog, buf_left);
		else
			return C;

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
	if (C == ' ' || C == '\t' || C == '\n' || C == '\r' || C == '\0')
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
long long get_int(char* buf)
{
	char* ptr = buf;
	long long integer = 0LL;
	while (digit(*ptr))
	{
		integer = integer * 10 + *ptr - '0';
		ptr++;
	}
	return integer;
}

int get_pnum_length(long long temp)
{
	int length = 1;
	while (temp > 10)
	{
		length++;
		temp /= 10;
	}
	return length;
}

double get_float(void)
{
	char* ptr = token;
	double  floatnum = 0;

	while (digit(*ptr))
	{
		floatnum = floatnum * 10 + *ptr - '0';
		ptr++;
	}

	if (*ptr == '.')
	{
		ptr++;
		long long temp = get_int(ptr);
		int length = get_pnum_length(temp);
		int power = pow(10, length);
		floatnum += (double)temp / power;
	}

	while (digit(*ptr)) ptr++;

	if (*ptr == 'E' || *ptr == 'e')
	{
		ptr++;
		int flag = 0;
		if (*ptr == '-')
		{
			flag = 1;
			ptr++;
		}
		else if (*ptr == '+')
			ptr++;

		long long num = get_int(ptr);
		if (flag)
			num = -num;
		double power = pow(10, num);
		floatnum *= power;
	}
	
	return floatnum;
}

//��ʶ����ļǺ���ӵ��Ǻ�������
void add(int type, int num)
{
	string word;
	long long integer;
	double floatnum;
	int index;
	Token temp;

	switch (type)
	{
	case WORD:
		//��ͨ��ʶ��
		word = token;
		index = Word.size();
		Word.push_back(word);
		temp.type = WORD;
		temp.index = index;
		temp.line = line;
		TOKEN.push_back(temp);
		cout << "<\tWORD\t\t" << word << "\t\t" << line << "\t>" << endl;
		break;
	case KEYWORD:
		//�ؼ���
		temp.type = KEYWORD;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
		cout << "<\tKEYWORD\t\t" << key[num] << "\t\t" << line << "\t>" << endl;
		break;
	case INTDIG:
		//����
		integer = get_int(token);
		index = INT.size();
		INT.push_back(integer);
		temp.type = INTDIG;
		temp.index = index;
		temp.line = line;
		TOKEN.push_back(temp);
		cout << "<\tINTDIG\t\t" << integer << "\t\t" << line << "\t>" << endl;
		break;
	case FLOATDIG:
		//������
		floatnum = get_float();
		index = FLOAT.size();
		FLOAT.push_back(floatnum);
		temp.type = FLOATDIG;
		temp.index = index;
		temp.line = line;
		TOKEN.push_back(temp);
		cout << "<\tFLOATDIG\t\t" << floatnum << "\t\t" << line << "\t>" << endl;
		break;
	case OPTION:
		//��������
		temp.type = OPTION;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
		cout << "<\tOPTION\t\t" << opt[num] << "\t\t" << line << "\t>" << endl;
		break;
	case END:
		//���
		temp.type = END;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
		cout << "<\tEND\t\t" << End[num] << "\t\t" << line << "\t>" << endl;
		break;
	case OTHER:
		//��������
		temp.type = OTHER;
		temp.index = num;
		temp.line = line;
		TOKEN.push_back(temp);
		cout << "<\tOTHER\t\t" << other[num] << "\t\t" << line << "\t>" << endl;
		break;
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
			<< "���󣺸�������С�����Ӧ��������" << endl
			<< "�кţ�" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Wrong_neg:
		cout << "****************************************************************************" << endl
			<< "���󣺸�����ָ�����ֵ�+��-�ź�Ӧ��������" << endl
			<< "�кţ�" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Wrong_E:
		cout << "****************************************************************************" << endl
			<< "���󣺸�����ָ������E��e��Ӧ��������ֻ�ܳ��֣�+/-������" << endl
			<< "�кţ�" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Invalid_character:
		cout << "****************************************************************************" << endl
			<< "���󣺳���C���Բ�֧�ֵ��ַ�" << endl
			<< "�кţ�" << line << endl
			<< "****************************************************************************" << endl;
		break;
	case Wrong_identifier:
		cout << "****************************************************************************" << endl
			<< "���󣺳��������ֿ�ͷ�ı�ʶ������ʶ��Ӧ����ĸ���»��߿�ͷ" << endl
			<< "�кţ�" << line << endl
			<< "****************************************************************************" << endl;
		break;
	default:
		cout << "****************************************************************************" << endl
			<< "���󣺱���������ʱ����" << endl
			<< "�кţ�" << line << endl
			<< "****************************************************************************" << endl;
		break;
	}
}