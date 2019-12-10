#include"main.h"

extern vector<Reduce> reduce_set;		//产生式集合
extern vector<Proj> proj_set;			//项目集合
extern vector<char> Terminator;		//终结符
extern vector<char> NonTerminator;		//非终结符
extern map<char, vector<char>> NonT_Follow;		//各非终结符的follow集
extern Reduce blank;
extern Reduce synch;
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;

extern int proj_num = 0;

void CreateProjSet(void)
{

}

//计算给定若干产生式集合的闭包
void Closure(vector<int> pID, vector<int>& projs)
{
	vector<int> temp;
	//将给定的若干产生式首先加入temp
	for (vector<int>::iterator it = pID.begin(); it != pID.end(); it++)
		temp.push_back(*it);

	//初始化：判断以某个非终结符作为产生式左部的所有产生式是否已经加入temp
	bool nonteradd[nonter_num] = { false };

	while (true)
	{
		//代表是否找到某个项目中的点后面是非终结符
		bool findnonter = false;

		//遍历所有已有的产生式
		for (vector<int>::iterator it = temp.begin(); it != temp.end(); it++)
		{
			//产生式的基本信息
			int rID = proj_set.at((*it)).rID;
			int dot = proj_set.at((*it)).dot_pos;
			if (dot == dot_end)
				continue;
			char ch = reduce_set.at(rID).right[dot];
			//判断产生式中点的后面是否是非终结符，且以它为左部的所有产生式是否已经加入temp
			if (BeNonTer(ch) && nonteradd[NonTer_to_num[ch]] == false)
			{
				findnonter = true;
				//找到所有以该终结符为左部的产生式
				vector<int>ch_left;
				findreduce(ch, ch_left);
				//将这些产生式并且点在开始位置的项目加入temp
				for (vector<int>::iterator it = ch_left.begin(); it != ch_left.end(); it++)
				{
					int pID = findproj(*it, 0);
					if (!Projexit(pID, temp))
						temp.push_back(pID);
				}
			}
		}

		//如果已经没有更新，则退出
		if (findnonter == false)
			break;
	}
	projs = temp;
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

void findreduce(char left, vector<int>& temp)
{
	for (vector<Reduce>::iterator it = reduce_set.begin(); it != reduce_set.end(); it++)
		if (left == (*it).left)
			temp.push_back((*it).rID);
}

int findproj(int rID, int dot_pos)
{
	for(vector<Proj>::iterator it = proj_set.begin() ; it != proj_set.end() ; it++)
		if (rID == (*it).rID && dot_pos == (*it).dot_pos)
			return (*it).pID;
}

bool Projexit(int pID, vector<int>temp)
{
	vector<int>::iterator it;
	it = find(temp.begin(), temp.end(), pID);
	if (it == temp.end())
		return false;
	else
		return true;
}



