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
	bool wrong = false;
	int ptr = 0;

	while (true)
	{
		char X = anastack.top();
		char a;
		if (digit(buffer[ptr]))
		{
			//简易的词法分析，处理有多位的数字，将其看做一位的数字
			while (digit(buffer[ptr + 1]))
				ptr++;

			a = 'n';
		}	
		else
			a = buffer[ptr];

		if (BeTer(X))
			//X是终结符或$
		{
			if (X == a)
			{
				//分析成功
				if (X == '$')
					break;

				anastack.pop();
				ptr++;
			}
			else
				//错误处理
			{
				wrong = true;
				cout << "栈顶符号与输入串符号不匹配，弹出栈顶符号" << endl;
				if (X != '$')
					anastack.pop();
			}
		}
		else
			//X是非终结符号
		{
			string temp = ana_table[NonTer_to_num[X]][Ter_to_num[a]].right;
			if (!(temp == "blank" || temp == "synch" ))
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
				wrong = true;
				if (temp == "blank")
				{
					cout << "分析表项为blank，前移输入串指针" << endl;
					if (buffer[ptr] == '$')
						anastack.pop();
					else
						ptr++;
				}
				else if (temp == "synch")
				{
					cout << "分析表项为synch，弹出栈顶符号" << endl;
					anastack.pop();
				}
			}
		}

		if (anastack.top() == '$')
			break;
	}

	if (anastack.top() == '$' && buffer[ptr] == '$' && !wrong)
		cout << "分析结束，分析成功" << endl;
	else
		cout << "分析结束，分析失败" << endl;
}

//判断字符是否是终结符
bool BeTer(char a)
{
	vector<char>::iterator it;
	it = find(Terminator.begin(), Terminator.end(), a);
	if (it == Terminator.end())
		return false;
	else
		return true;
}

//判断字符是否是非终结符
bool BeNonTer(char a)
{
	vector<char>::iterator it;
	it = find(NonTerminator.begin(), NonTerminator.end(), a);
	if (it == NonTerminator.end())
		return false;
	else
		return true;
}

//判断字符是否是数字
bool digit(char a)
{
	if (a >= '0' && a <= '9')
		return true;
	else
		return false;
}