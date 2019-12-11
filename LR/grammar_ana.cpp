#include"main.h"

extern vector<Reduce> reduce_set;		//产生式集合
extern vector<Proj> proj_set;			//项目集合
extern vector<char> Terminator;		//终结符
extern vector<char> NonTerminator;		//非终结符
extern map<char, vector<char>> NonT_Follow;		//各非终结符的follow集
extern Table_entry blank;
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
extern vector<ProjS> proj_cluster;
extern int proj_set_num;
extern Table_entry ana_table[max_state][ter_num + nonter_num - 1];

stack<int> statestack;					//状态栈
stack<char> chstack;					//符号栈
extern char buffer[max_input];			//输入串
extern bool print;

//文法分析
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
		cout << "分析栈";
		cout.width(30);
		cout << "输入缓存区";
		cout << "\t\t语法分析器输出" << endl << endl;
	}

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
			//X是终结符或$
		{
			if (X == a)
			{
				//分析成功
				if (X == '$')
					break;

				anastack.pop();
				ptr++;
				now = ptr;
				cout << "终结符匹配成功" << endl;
			}
			else
				//错误处理
			{
				wrong = true;
				cout << "栈顶符号与输入串符号不匹配，弹出栈顶符号" << endl;
				if (X != '$')
					anastack.pop();

				ErrorControl(a, X);
			}
		}
		else
			//X是非终结符号
		{
			string temp = ana_table[NonTer_to_num[X]][Ter_to_num[a]].right;
			if (!(temp == "blank" || temp == "synch"))
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
					{
						ptr++;
						now = ptr;
					}
				}
				else if (temp == "synch")
				{
					cout << "分析表项为synch，弹出栈顶符号" << endl;
					anastack.pop();
				}
				ErrorControl(a, X);
			}
		}

		if (anastack.top() == '$')
			break;
	}

	if (anastack.top() == '$' && buffer[ptr] == '$' && !wrong)
		cout << endl << endl << "分析结束，分析成功" << endl << endl;
	else
		cout << endl << endl << "分析结束，分析失败" << endl << endl;
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

//打印错误处理信息
void ErrorControl(char ter, char nont)
{
	if ((nont == 'E' || nont == 'T') && (ter == '+' || ter == '-' || ter == '*' || ter == '/'))
		cout << "可能缺少数字，或输入错误" << endl;
	else if ((nont == 'E' || nont == 'T') && (ter == ')' || ter == '$'))
		cout << "输入的算数表达式不完整" << endl;
	else if (nont == 'A' && (ter == '(' || ter == 'n'))
		cout << "可能缺少运算符号" << endl;
	else if (nont == 'B' && (ter == '(' || ter == 'n'))
		cout << "可能缺少运算符号" << endl;
	else if (nont == 'F' && ter == ')')
		cout << "括号输入错误" << endl;
	else if (nont == 'F' && ter == '$')
		cout << "输入的算数表达式不完整" << endl;
	else if (nont == 'F' && ter == '$')
		cout << "输入的算数表达式不完整" << endl;
	else if (nont == 'F' && ter == '$')
		cout << "可能缺少数字，或输入错误" << endl;

	if (nont == '(' || nont == ')' || ter == '(' || ter == ')')
		cout << "括号输入错误" << endl;
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