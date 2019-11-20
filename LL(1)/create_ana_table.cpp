#include"main.h"

extern vector<Reduce> reduce_set;		//产生式集合
extern vector<char> Terminator;			//终结符
extern vector<char> NonTerminator;		//非终结符
extern map<char, vector<char>> NonT_Follow;		//各非终结符的follow集
extern Reduce ana_table[nonter_num][ter_num];	//预测分析表
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
extern Reduce blank;
extern Reduce synch;

void create_ana_table(void)
{
	//预处理：先将分析表中所有位置都填上空白
	for (int i = 0; i < nonter_num; i++)
		for (int j = 0; j < ter_num; j++)
			ana_table[i][j] = blank;

	//遍历所有产生式	A -> ......
	vector<Reduce>::iterator it;
	for (it = reduce_set.begin(); it != reduce_set.end(); it++)
	{
		//遍历所有终结符
		vector<char>::iterator a;
		for (a = Terminator.begin(); a != Terminator.end(); a++)
		{
			//判断该终结符是否在产生式右部的first集中
			vector<char>::iterator result = find((*it).first.begin(), (*it).first.end(), *a);

			//若在，将产生式加入到分析表对应位置中
			if (result != (*it).first.end())
				ana_table[NonTer_to_num[(*it).left]][Ter_to_num[*a]] = *it;
		}

		//判断产生式右部的first集中有无epsilon
		vector<char>::iterator result = find((*it).first.begin(), (*it).first.end(), 'e');
		//若epsilon属于该产生式右部的first集
		if (result != (*it).first.end())
			//将该产生式放入A的follow集中元素对应的位置
		{
			vector<char>::iterator b;
			for (b = NonT_Follow[(*it).left].begin(); b != NonT_Follow[(*it).left].end(); b++)
				ana_table[NonTer_to_num[(*it).left]][Ter_to_num[*b]] = *it;
		}
	}

	//填入同步信息synch
	vector<char>::iterator Tit;
	vector<char>::iterator Nit;
	for (Nit = NonTerminator.begin(); Nit != NonTerminator.end(); Nit++)
	{
		for (Tit = Terminator.begin(); Tit != Terminator.end(); Tit++)
		{
			vector<char>::iterator re = find(NonT_Follow[*Nit].begin(), NonT_Follow[*Nit].end(), *Tit);

			if (re != NonT_Follow[*Nit].end() && ana_table[NonTer_to_num[*Nit]][Ter_to_num[*Tit]].right == "blank")
				ana_table[NonTer_to_num[*Nit]][Ter_to_num[*Tit]] = synch;
		}
	}
}

//打印分析表
void print_ana_table(void)
{
	for (int i = 0; i < nonter_num; i++)
	{
		for (int j = 0; j < ter_num; j++)
		{
			if (ana_table[i][j].right == "blank")
			{
				cout.width(15);
				cout << "Blank";
			}
			else if (ana_table[i][j].right == "synch")
			{
				cout.width(15);
				cout << "Synch";
			}
			else
			{
				cout.width(15);
				string left = &ana_table[i][j].left;
				string output = left + " -> " + ana_table[i][j].right;
				cout << output;
			}
		}
		cout << endl;
	}
}