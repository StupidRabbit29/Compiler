#include"main.h"

extern vector<Reduce> reduce_set;		//����ʽ����
extern vector<Proj> proj_set;			//��Ŀ����
extern vector<char> Terminator;		//�ս��
extern vector<char> NonTerminator;		//���ս��
extern map<char, vector<char>> NonT_Follow;		//�����ս����follow��
extern Table_entry blank;
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
extern vector<ProjS> proj_cluster;
extern int proj_set_num;
extern Table_entry ana_table[max_state][ter_num + nonter_num - 1];

stack<int> statestack;					//״̬ջ
stack<char> chstack;					//����ջ
extern char buffer[max_input];			//���봮
extern bool print;

//�ķ�����
void Grammar_ana(void)
{
	statestack.push(0);
	chstack.push('_');
	bool wrong = false;
	int ptr = 0;
	int now = 0;

	if (print)
	{
		cout << endl << endl;
		cout.width(30);
		cout << "����ջ";
		cout.width(30);
		cout << "���뻺����";
		cout << "\t\t�﷨���������" << endl << endl;
	}

	while (true)
	{
		char X = anastack.top();
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
			print_stack();
			cout.width(30);
			char temp[max_input] = { '\0' };
			strcpy_s(temp, &(buffer[now]));
			cout << temp;
			cout << "\t\t";
		}

		if (BeTer(X))
			//X���ս����$
		{
			if (X == a)
			{
				//�����ɹ�
				if (X == '$')
					break;

				anastack.pop();
				ptr++;
				now = ptr;
				cout << "�ս��ƥ��ɹ�" << endl;
			}
			else
				//������
			{
				wrong = true;
				cout << "ջ�����������봮���Ų�ƥ�䣬����ջ������" << endl;
				if (X != '$')
					anastack.pop();

				ErrorControl(a, X);
			}
		}
		else
			//X�Ƿ��ս����
		{
			string temp = ana_table[NonTer_to_num[X]][Ter_to_num[a]].right;
			if (!(temp == "blank" || temp == "synch"))
				//�ж�Ӧ�Ĳ���ʽ
			{
				string reduce_r = ana_table[NonTer_to_num[X]][Ter_to_num[a]].right;
				anastack.pop();
				for (int i = reduce_r.length() - 1; i > -1; i--)
					anastack.push(reduce_r[i]);
				cout << X << " --> " << reduce_r << endl;
			}
			else
				//������
			{
				wrong = true;
				if (temp == "blank")
				{
					cout << "��������Ϊblank��ǰ�����봮ָ��" << endl;
					if (buffer[ptr] == '$')
						anastack.pop();
					else
					{
						ptr++;
						now = ptr;
					}
				}
				else if (temp == "synch")
				{
					cout << "��������Ϊsynch������ջ������" << endl;
					anastack.pop();
				}
				ErrorControl(a, X);
			}
		}

		if (anastack.top() == '$')
			break;
	}

	if (anastack.top() == '$' && buffer[ptr] == '$' && !wrong)
		cout << endl << endl << "���������������ɹ�" << endl << endl;
	else
		cout << endl << endl << "��������������ʧ��" << endl << endl;
}

//�ж��ַ��Ƿ����ս��
bool BeTer(char a)
{
	vector<char>::iterator it;
	it = find(Terminator.begin(), Terminator.end(), a);
	if (it == Terminator.end())
		return false;
	else
		return true;
}

//�ж��ַ��Ƿ��Ƿ��ս��
bool BeNonTer(char a)
{
	vector<char>::iterator it;
	it = find(NonTerminator.begin(), NonTerminator.end(), a);
	if (it == NonTerminator.end())
		return false;
	else
		return true;
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
void ErrorControl(char ter, char nont)
{
	if ((nont == 'E' || nont == 'T') && (ter == '+' || ter == '-' || ter == '*' || ter == '/'))
		cout << "����ȱ�����֣����������" << endl;
	else if ((nont == 'E' || nont == 'T') && (ter == ')' || ter == '$'))
		cout << "������������ʽ������" << endl;
	else if (nont == 'A' && (ter == '(' || ter == 'n'))
		cout << "����ȱ���������" << endl;
	else if (nont == 'B' && (ter == '(' || ter == 'n'))
		cout << "����ȱ���������" << endl;
	else if (nont == 'F' && ter == ')')
		cout << "�����������" << endl;
	else if (nont == 'F' && ter == '$')
		cout << "������������ʽ������" << endl;
	else if (nont == 'F' && ter == '$')
		cout << "������������ʽ������" << endl;
	else if (nont == 'F' && ter == '$')
		cout << "����ȱ�����֣����������" << endl;

	if (nont == '(' || nont == ')' || ter == '(' || ter == ')')
		cout << "�����������" << endl;
}

void print_stack(void)
{
	stack<char>temp;
	char buf[200] = { '\0' };
	int i = 0;

	while (!anastack.empty())
	{
		temp.push(anastack.top());
		anastack.pop();
	}

	while (!temp.empty())
	{
		anastack.push(temp.top());
		buf[i] = temp.top();
		i++;
		temp.pop();
	}

	cout.width(30);
	cout << buf;
}