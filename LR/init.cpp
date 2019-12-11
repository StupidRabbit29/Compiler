#include"main.h"

vector<Reduce> reduce_set;		//产生式集合
vector<Proj> proj_set;			//项目集合
vector<char> Terminator;		//终结符
vector<char> NonTerminator;		//非终结符
map<char, vector<char>> NonT_Follow;		//各非终结符的follow集
Reduce blank;
Reduce synch;
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
	{ 'S', 0 },
	{ 'E', 1 },
	{ 'T', 2 },
	{ 'F', 3 }
};

int proj_num = 0;

//初始化，输入文法的基本信息
void init(void)
{
	Reduce r1;
	r1.rID = 0;
	r1.left = 'S';
	r1.right = "E";
	reduce_set.push_back(r1);

	Reduce r2;
	r2.rID = 1;
	r2.left = 'E';
	r2.right = "E+T";
	reduce_set.push_back(r2);

	Reduce r3;
	r3.rID = 2;
	r3.left = 'E';
	r3.right = "E-T";
	reduce_set.push_back(r3);

	Reduce r4;
	r4.rID = 3;
	r4.left = 'E';
	r4.right = "T";
	reduce_set.push_back(r4);

	Reduce r5;
	r5.rID = 4;
	r5.left = 'T';
	r5.right = "T*F";
	reduce_set.push_back(r5);

	Reduce r6;
	r6.rID = 5;
	r6.left = 'T';
	r6.right = "T/F";
	reduce_set.push_back(r6);

	Reduce r7;
	r7.rID = 6;
	r7.left = 'T';
	r7.right = "F";
	reduce_set.push_back(r7);

	Reduce r8;
	r8.rID = 7;
	r8.left = 'F';
	r8.right = "(E)";
	reduce_set.push_back(r8);

	Reduce r9;
	r9.rID = 8;
	r9.left = 'F';
	r9.right = "n";
	reduce_set.push_back(r9);

	char ter[ter_num] = { '+', '-', '*', '/', '(', ')', 'n', '$' };	//终结符
	char nonter[nonter_num] = { 'S', 'E', 'T', 'F' };				//非终结符

	for (int i = 0; i < ter_num; i++)
		Terminator.push_back(ter[i]);
	for (int i = 0; i < nonter_num; i++)
		NonTerminator.push_back(nonter[i]);

	//非终结符的Follow集
	vector<char> S{ '$' };
	vector<char> E{ '$', '+', '-', ')' };
	vector<char> T{ '$', ')', '+', '-', '*', '/' };
	vector<char> F{ '$', ')', '+', '-', '*', '/' };

	NonT_Follow['S'] = S;
	NonT_Follow['E'] = E;
	NonT_Follow['T'] = T;
	NonT_Follow['F'] = F;

	//blank.right = "blank";
	//synch.right = "synch";

	Produce_Proj();
}

//构造所有项目
void Produce_Proj(void)
{
	vector<Reduce>::iterator it, begin;
	begin = reduce_set.begin();

	//遍历所有产生式
	for (it = reduce_set.begin(); it != reduce_set.end(); it++)
	{
		//遍历点可以出现在产生式中的位置
		for (int i = 0; i < (*it).right.length(); i++)
		{
			Proj temp;
			temp.pID = proj_num;
			temp.rID = it - begin;
			temp.dot_pos = i;
			proj_set.push_back(temp);
			proj_num++;
		}

		//点可以出现在产生式最后，即规约项目
		Proj temp;
		temp.pID = proj_num;
		temp.rID = it - begin;
		temp.dot_pos = dot_end;
		proj_set.push_back(temp);
		proj_num++;
	}
}