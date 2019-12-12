#include"main.h"

extern vector<Reduce> reduce_set;		//����ʽ����
extern Table_entry blank;
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
extern Table_entry ana_table[max_state][ter_num + nonter_num - 1];

stack<int> statestack;					//״̬ջ
stack<char> chstack;					//����ջ
extern char buffer[max_input];			//���봮
extern bool print;

string printErr[4] = { "ȱ���������", "ȱ���������", "���Ų�ƥ��", "��������" };

//�ķ�����
void Grammar_ana(void)
{
	statestack.push(0);
	chstack.push(' ');
	bool wrong = false;
	int ptr = 0;
	int now = 0;

	if (print)
	{
		cout << endl << endl;
		cout.width(50);
		cout << "����ջ";
		cout.width(30);
		cout << "���뻺����";
		cout << "\t\t�﷨���������" << endl << endl;
	}

	while (true)
	{
		if (statestack.empty())
		{
			wrong = true;
			break;
		}
		int state = statestack.top();
		char X = chstack.top();
		char a;
		if (digit(buffer[ptr]))
		{
			//���׵Ĵʷ������������ж�λ�����֣����俴��һλ������
			while (digit(buffer[ptr + 1]))
				ptr++;

			a = 'n';
		}
		else
			a = buffer[ptr];

		if (print)
		{
			cout.width(50);
			cout << print_stack(statestack);
			char temp[max_input] = { '\0' };
			strcpy_s(temp, &(buffer[now]));
			cout.width(30);
			cout << temp;
			//cout.setf(ios_base::right);
			cout << "\t\t";
		}
		string printtemp = print_stack(chstack);

		Table_entry work = ana_table[state][Ter_to_num[a]];

		if (work.type == Shiftto)
		{
			statestack.push(work.state);
			chstack.push(a);
			ptr++;
			now = ptr;

			cout << "Shift to state " << work.state << endl;
			if (print)
			{
				cout.width(50);
				cout << printtemp << endl;
			}
		}
		else if (work.type == Reduceby)
		{
			Reduce red = reduce_set.at(work.rID);
			int len = red.right.length();
			for (int i = 0; i < len; i++)
			{
				statestack.pop();
				chstack.pop();
			}

			int newstate = statestack.top();
			statestack.push(ana_table[newstate][NonTer_to_num[red.left] + ter_num - 1].state);
			chstack.push(red.left);

			//cout << red.left << " --> " << red.right << endl;
			cout << "Reduce by " << red.left << " --> " << red.right << endl;
			if (print)
			{
				cout.width(50);
				cout << printtemp << endl;
			}
		}
		else if (work.type == ACC)
		{
			cout << "ACC" << endl;
			if (print)
			{
				cout.width(50);
				cout << printtemp << endl;
			}
			break;
		}
		else if (work.type == Error)
		{
			wrong = true;
			cout << printErr[work.errortype] << endl;
			//ErrorControl(work.errortype);
			break;000
		}
	}

	if (!wrong)
		cout << endl << endl << "���������������ɹ�" << endl << endl;
	else
		cout << endl << endl << "��������������ʧ��" << endl << endl;
}

//�ж��ַ��Ƿ�������
bool digit(char a)
{
	if (a >= '0' && a <= '9')
		return true;
	else
		return false;
}

//��ӡ��������Ϣ
//void ErrorControl(int Err)
//{
//	if (Err == e1)
//	{
//		statestack.push(2);
//		chstack.push('n');
//	}
//	else if (Err == e2)
//	{
//
//	}
//	else if (Err == e3)
//	{
//
//	}
//	else if (Err == e4)
//	{
//
//	}
//}

template<class Type>
string print_stack(stack<Type> work)
{
	stringstream ss;
	string result;

	stack<Type>temp;
	Type buf[200] = { '\0' };
	int i = 0;

	while (!work.empty())
	{
		temp.push(work.top());
		work.pop();
	}

	while (!temp.empty())
	{
		work.push(temp.top());
		buf[i] = temp.top();
		i++;
		temp.pop();
	}

	//cout.width(40);
	for (int j = 0; j < i; j++)
	{
		ss.fill(' ');
		ss.width(3);
		ss << buf[j];
	}
	result = ss.str();
	//cout << result.length() << endl;
	return result;
}