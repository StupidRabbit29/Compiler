#include"main.h"

vector<Reduce> reduce_set;		//����ʽ����
vector<char> Terminator;		//�ս��
vector<char> NonTerminator;		//���ս��
map<char, vector<char>> NonT_Follow;		//�����ս����follow��
Reduce ana_table[nonter_num][ter_num];		//Ԥ�������
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
	cout << "�������������ʽ��" << endl;
	cin >> buffer;
	buffer[strlen(buffer)] = '$';
}