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
char buffer[max_input] = { '\0' };

int main(void)
{
	init();
	create_ana_table();
	print_ana_table();
	get_input();
	Grammar_ana();

	return 0;
}

void get_input(void)
{
	cout << "请输入算数表达式：" << endl;
	cin >> buffer;
	buffer[strlen(buffer)] = '$';
}