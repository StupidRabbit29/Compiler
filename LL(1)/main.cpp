#include"main.h"

vector<Reduce> reduce_set;		//产生式集合
vector<char> Terminator;		//终结符
vector<char> NonTerminator;		//非终结符
map<char, vector<char>> NonT_Follow;		//各非终结符的follow集
Reduce ana_table[nonter_num][ter_num];		//预测分析表
map<char, int> Ter_to_num = {
	{ '+', 0 },
	{ '-', 1 },
	{ '*', 2 },
	{ '/', 3 },
	{ '(', 4 },
	{ ')', 5 },
	{ 'n', 6 },
	{ '$', 7 },
};
map<char, int> NonTer_to_num = {
	{ 'E', 0 },
	{ 'T', 1 },
	{ 'A', 2 },
	{ 'F', 3 },
	{ 'B', 4 },
};
char buffer[max_input] = { '\0' };			//输入缓冲区
bool print = false;		//是否打印更多分析过程

void Getarg(int argc, char* argv[])
{
	if (argc == 2)
	{
		char* input = argv[1];
		if (strcmp(input, "-d") == 0)
			print = true;
	}
}

int main(int argc, char* argv[])
{
	Getarg(argc, argv);		//处理用户输入
	init();					//初始化文法信息
	create_ana_table();		//构造分析表
	print_ana_table();		//打印分析表
	get_input();			//获取用户输入的字符串
	Grammar_ana();			//对字符串进行语法分析

	return 0;
}

void get_input(void)
{
	cout << "请输入算数表达式：" << endl;
	cin >> buffer;
	buffer[strlen(buffer)] = '$';
}