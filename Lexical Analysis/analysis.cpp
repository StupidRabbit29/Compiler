#include"main.h"
using namespace std;


extern char buf[half_buf_size * 2];
extern char* lexemebegin;
extern char* forwardp;
extern char token[half_buf_size];

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
			if (C >= 'a' && C <= 'z' || C >= 'A' && C <= 'Z')
			{
				//�����ʶ��״̬
				state = 1;
				break;
			}
			else if (C >= '0' && C <= '9')
			{
				//��������״̬
				state = 2;
				break;
			}
			else
				switch (C)
				{
				
				}
		case 1://��ʶ��״̬
			token[cur] = C;
			cur++;
			C = get_char(inprog);
			//�ж��Ƿ��Ǻϸ���ַ������ڳ�ǰ��֤
			if (letter(C) || digit(C) || C == '_')
				state = 1;
			else
				//�Ѵﵽ��ʶ����β
			{

			}
		case 2://����״̬
		case 3://С����״̬
		case 4://С��״̬
		case 5://ָ��״̬
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
