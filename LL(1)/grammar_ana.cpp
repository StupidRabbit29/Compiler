#include"main.h"

extern vector<char> Terminator;			//�ս��
extern vector<char> NonTerminator;		//���ս��
extern Reduce ana_table[nonter_num][ter_num];	//Ԥ�������
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
stack<char> anastack;					//����ջ
extern char buffer[max_input];			//���봮

void Grammar_ana(void)
{
	anastack.push('$');
	anastack.push('E');
	bool wrong = false;
	int ptr = 0;

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
			}
			else
				//������
			{
				wrong = true;
				cout << "ջ�����������봮���Ų�ƥ�䣬����ջ������" << endl;
				if (X != '$')
					anastack.pop();
			}
		}
		else
			//X�Ƿ��ս����
		{
			string temp = ana_table[NonTer_to_num[X]][Ter_to_num[a]].right;
			if (!(temp == "blank" || temp == "synch" ))
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
						ptr++;
				}
				else if (temp == "synch")
				{
					cout << "��������Ϊsynch������ջ������" << endl;
					anastack.pop();
				}
			}
		}

		if (anastack.top() == '$')
			break;
	}

	if (anastack.top() == '$' && buffer[ptr] == '$' && !wrong)
		cout << "���������������ɹ�" << endl;
	else
		cout << "��������������ʧ��" << endl;
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