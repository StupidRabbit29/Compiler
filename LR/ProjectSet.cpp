#include"main.h"

extern vector<Reduce> reduce_set;		//产生式集合
extern vector<Proj> proj_set;			//项目集合
extern vector<char> Terminator;		//终结符
extern vector<char> NonTerminator;		//非终结符
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
vector<ProjS> proj_cluster;
stack<ProjS> todo;
int proj_set_num = 0;

//产生项目集规范簇
void CreateProjSet(void)
{
	vector<ProjS> exist;
	vector<int> pID;
	int start = findproj(0, 0);
	pID.push_back(start);
	ProjS Start;
	Closure(pID, Start.projs);
	Start.sID = proj_set_num++;
	todo.push(Start);
	exist.push_back(Start);

	while (!todo.empty())
	{
		ProjS work = todo.top();
		todo.pop();	
			
		bool action[ter_num] = { false };
		bool Goto[nonter_num] = { false };
		ProjS actionProj[ter_num];
		ProjS gotoProj[nonter_num];
		for (vector<int>::iterator it = work.projs.begin(); it != work.projs.end(); it++)
		{
			Proj temp = proj_set.at(*it);
			int rID = temp.rID;
			string right = reduce_set.at(rID).right;
			int dot_pos;
			if (temp.dot_pos != -1)
				dot_pos = temp.dot_pos;
			else
				continue;
			char ch = right[dot_pos];
			if (BeTer(ch))
			{
				int tern = Ter_to_num[ch];
				if (!action[tern])
					action[tern] = true;

				if (dot_pos == right.length()-1)
					actionProj[tern].projs.push_back(findproj(rID, dot_end));
				else
					actionProj[tern].projs.push_back(findproj(rID, dot_pos + 1));
			}
			else
			{
				int nontern = NonTer_to_num[ch];
				if (Goto[nontern] == false)
					Goto[nontern] = true;

				if (dot_pos == right.length() - 1)
					gotoProj[nontern].projs.push_back(findproj(rID, dot_end));
				else
					gotoProj[nontern].projs.push_back(findproj(rID, dot_pos + 1));
			}
		}

		for (int i = 0; i < ter_num; i++)
		{
			if (action[i])
			{
				Closure(actionProj[i].projs, actionProj[i].projs);
				if (!checkSameProjS(actionProj[i], exist))
				{
					actionProj[i].sID = proj_set_num;
					proj_set_num++;
					todo.push(actionProj[i]);
					exist.push_back(actionProj[i]);
					PprojS ptr;
					ptr.sID = actionProj[i].sID;
					ptr.word = Terminator.at(i);
					work.ptr.push_back(ptr);
				}
				else
				{
					PprojS ptr;
					ptr.sID = findsID(actionProj[i], exist);
					ptr.word = Terminator.at(i);
					work.ptr.push_back(ptr);
				}
			}
		}

		for (int i = 0; i < nonter_num; i++)
		{
			if (Goto[i])
			{
				Closure(gotoProj[i].projs, gotoProj[i].projs);
				if (!checkSameProjS(gotoProj[i], exist))
				{
					gotoProj[i].sID = proj_set_num;
					proj_set_num++;
					todo.push(gotoProj[i]);
					exist.push_back(gotoProj[i]);
					PprojS ptr;
					ptr.sID = gotoProj[i].sID;
					ptr.word = NonTerminator.at(i);
					work.ptr.push_back(ptr);
				}
				else
				{
					PprojS ptr;
					ptr.sID = findsID(gotoProj[i], exist);
					ptr.word = NonTerminator.at(i);
					work.ptr.push_back(ptr);
				}
			}
		}

		proj_cluster.push_back(work);
	}
}

void PrintProjs(void)
{
	cout << "状态总数：" << proj_cluster.size() << endl << endl;

	for (vector<ProjS>::iterator it = proj_cluster.begin(); it != proj_cluster.end(); it++)
	{
		cout << "--------------------" << endl;
		cout << "状态：" << (*it).sID << endl << endl;
		cout << "项目：" << endl;
		for (vector<int>::iterator pit = (*it).projs.begin(); pit != (*it).projs.end(); pit++)
		{
			Proj proj = proj_set.at(*pit);
			int rID = proj.rID;
			int dot_pos = proj.dot_pos;
			char left = reduce_set.at(rID).left;
			string right = reduce_set.at(rID).right;
			if (dot_pos != dot_end && dot_pos != 0)
				cout << left << " --> " << right.substr(0, dot_pos) << " . " << right.substr(dot_pos, right.length() - dot_pos) << endl;
			else if (dot_pos == 0)
				cout << left << " --> " << " . " << right << endl;
			else
				cout << left << " --> " << right << " . " << endl;
		}
		cout << endl;

		cout << "跳转" << endl;
		for (vector<PprojS>::iterator qit = (*it).ptr.begin(); qit != (*it).ptr.end(); qit++)
		{
			cout << (*qit).word << " : " << (*qit).sID << endl;
		}
		cout << "--------------------" << endl << endl;
	}
}

//检测项目集是否已经存在
bool checkSameProjS(ProjS& a, vector<ProjS>& exist)
{
	bool same = false;
	for (vector<ProjS>::iterator it = exist.begin(); it != exist.end(); it++)
		if (checkSameProj(a, *it))
			same = true;

	return same;
}

int findsID(ProjS& a, vector<ProjS>& exist)
{
	for (vector<ProjS>::iterator it = exist.begin(); it != exist.end(); it++)
		if (checkSameProj(a, *it))
			return (*it).sID;
}

bool checkSameProj(ProjS& a, ProjS& b)
{
	if (a.projs == b.projs)
		return true;
	else
		return false;
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
		vector<int>::iterator it = temp.begin();
		while (true)
		{
			//产生式的基本信息
			int rID = proj_set.at((*it)).rID;
			int dot = proj_set.at((*it)).dot_pos;
			if (dot == dot_end)
			{
				it++;
				if (it == temp.end())
					break;
				continue;
			}
			char ch = reduce_set.at(rID).right[dot];
			//判断产生式中点的后面是否是非终结符，且以它为左部的所有产生式是否已经加入temp
			if (BeNonTer(ch) && nonteradd[NonTer_to_num[ch]] == false)
			{
				findnonter = true;
				nonteradd[NonTer_to_num[ch]] = true;
				bool add = false;
				//找到所有以该终结符为左部的产生式
				vector<int>ch_left;
				findreduce(ch, ch_left);
				//将这些产生式并且点在开始位置的项目加入temp
				for (vector<int>::iterator rit = ch_left.begin(); rit != ch_left.end(); rit++)
				{
					int pID = findproj(*rit, 0);
					if (!Projexit(pID, temp))
					{
						add = true;
						temp.push_back(pID);
					}
				}
				if (add)
					it = temp.begin();
				else
					it++;
			}
			else
				it++;

			if (it == temp.end())
				break;
		}

		//如果已经没有更新，则退出
		if (findnonter == false)
			break;
	}
	projs = temp;
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



