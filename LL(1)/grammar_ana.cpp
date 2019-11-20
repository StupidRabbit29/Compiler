#include"main.h"

extern vector<char> Terminator;			//终结符
extern vector<char> NonTerminator;		//非终结符
extern Reduce ana_table[nonter_num][ter_num];	//预测分析表
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
stack<char> anastack;					//分析栈
extern char buffer[max_input];			//输入串

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
			//X是终结符或$
		{
			if (X == a)
			{
				anastack.pop();
				ptr++;
			}
			else
				//错误处理
			{

			}
		}
		else
			//X是非终结符号
		{
			if (!(ana_table[NonTer_to_num[X]][Ter_to_num[a]].right == "blank"))
				//有对应的产生式
			{
				string reduce_r = ana_table[NonTer_to_num[X]][Ter_to_num[a]].right;
				anastack.pop();
				for (int i = reduce_r.length() - 1; i > -1; i--)
					anastack.push(reduce_r[i]);
				cout << X << " --> " << reduce_r << endl;
			}
			else
				//错误处理
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