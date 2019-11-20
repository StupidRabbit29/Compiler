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
	int ptr = 0;

	while (true)
	{
		char X = anastack.top();
		char a;
		if (digit(buffer[ptr]))
			a = 'n';
		else
			a = buffer[ptr];

		if (X == '$' || BeTer(X))
			//X���ս����$
		{
			if (X == a)
			{
				anastack.pop();
				ptr++;
			}
			else
				//������
			{

			}
		}
		else
			//X�Ƿ��ս����
		{
			if (!(ana_table[NonTer_to_num[X]][Ter_to_num[a]].right == "blank"))
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

			}
		}

		if (anastack.top() == '$')
			break;
	}
}

bool BeTer(char a)
{
	vector<char>::iterator it;
	it = find(Terminator.begin(), Terminator.end(), a);
	if (it == Terminator.end())
		return false;
	else
		return true;
}

bool BeNonTer(char a)
{
	vector<char>::iterator it;
	it = find(NonTerminator.begin(), NonTerminator.end(), a);
	if (it == NonTerminator.end())
		return false;
	else
		return true;
}

bool digit(char a)
{
	if (a >= '0' && a <= '9')
		return true;
	else
		return false;
}