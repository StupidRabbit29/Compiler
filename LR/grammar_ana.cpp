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
	chstack.push(' ');
	bool wrong = false;
	int ptr = 0;
	int now = 0;

	if (print)
	{
		cout << endl << endl;
		cout.width(50);
		cout << "分析栈";
		cout.width(30);
		cout << "输入缓存区";
		cout << "\t\t语法分析器输出" << endl << endl;
	}

	while (true)
	{
		int state = statestack.top();
		char X = chstack.top();
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

		Table_entry work;

		if (BeTer(a))
			work = ana_table[state][Ter_to_num[a]];
		else
			work = ana_table[state][NonTer_to_num[a] + ter_num - 1];

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
				cout << print_stack(chstack) << endl;
			}
		}
		else if (work.type == ACC)
		{
			cout << "ACC" << endl;
			if (print)
			{
				cout.width(50);
				cout << print_stack(chstack) << endl;
			}
			break;
		}
		else
		{
			wrong = true;
		}
	}
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