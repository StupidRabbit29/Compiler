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
Table_entry ana_table[max_state][ter_num + nonter_num - 1];



void create_ana_table(void)
{
	int line = proj_set_num;
	int row = ter_num + nonter_num - 1;
	//预处理：先将分析表中所有位置都填上空白
	for (int i = 0; i < line; i++)
		for (int j = 0; j < row; j++)
			ana_table[i][j] = blank;

	for (vector<ProjS>::iterator it = proj_cluster.begin(); it != proj_cluster.end(); it++)
	{
		ProjS work = *it;
		int sID = work.sID;
		for (vector<PprojS>::iterator pit = work.ptr.begin(); pit != work.ptr.end(); pit++)
		{
			char ch = (*pit).word;
			if (BeTer(ch))
			{
				Table_entry temp;
				temp.type = Shiftto;
				temp.state = (*pit).sID;
				ana_table[sID][Ter_to_num[ch]] = temp;
			}			
			else
			{
				Table_entry temp;
				temp.type = Gotostate;
				temp.state = (*pit).sID;
				ana_table[sID][NonTer_to_num[ch]+ter_num-1] = temp;
			}
		}

		for (vector<int>::iterator pit = work.projs.begin(); pit != work.projs.end(); pit++)
		{
			if (proj_set.at(*pit).dot_pos == -1)
			{
				if (reduce_set.at(proj_set.at(*pit).rID).rID == 0)
				{
					Table_entry temp;
					temp.type = ACC;
					ana_table[sID][Ter_to_num['$']] = temp;
				}
				else
				{
					Table_entry temp;
					temp.type = Reduceby;
					temp.rID = reduce_set.at(proj_set.at(*pit).rID).rID;
					char left = reduce_set.at(proj_set.at(*pit).rID).left;
					for (vector<char>::iterator cit = NonT_Follow[left].begin(); cit != NonT_Follow[left].end(); cit++)
						ana_table[sID][Ter_to_num[*cit]] = temp;
				}
			}
		}
	}
	
}

//打印分析表
void print_ana_table(void)
{
	int line = proj_set_num;
	int row = ter_num + nonter_num - 1;

	cout << "********************************************************分析表********************************************************" << endl;
	cout << "  " ;
	for (int j = 0; j < ter_num; j++)
	{
		cout.width(10);
		cout << Terminator.at(j);
	}
	for (int j = 0; j < nonter_num - 1; j++)
	{
		cout.width(10);
		cout << NonTerminator.at(j+1);
	}
	cout << endl;

	for (int i = 0; i < line; i++)
	{
		cout.width(2);
		cout << i;

		for (int j = 0; j < row; j++)
		{
			stringstream ss;
			if (ana_table[i][j].type == Blank)
			{
				cout.width(10);
				cout << " ";
			}
			else if (ana_table[i][j].type == Shiftto)
			{
				cout.width(10);
				string temp;
				ss << ana_table[i][j].state;
				ss >> temp;
				temp = "S" + temp;
				cout << temp;
			}
			else if (ana_table[i][j].type == Reduceby)
			{
				cout.width(10);
				string temp;
				ss << ana_table[i][j].rID;
				ss >> temp;
				temp = "R" + temp;
				cout << temp;
			}
			else if (ana_table[i][j].type == Gotostate)
			{
				cout.width(10);
				string temp;
				ss << ana_table[i][j].state;
				ss >> temp;
				temp = "Go" + temp;
				cout << temp;
			}
			else if (ana_table[i][j].type == ACC)
			{
				cout.width(10);
				cout << "ACC";
			}
		}
		cout << endl;
	}
	cout << "**********************************************************************************************************************" << endl;
	cout << endl << endl;
}