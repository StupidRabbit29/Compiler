#include"main.h"

vector<Reduce> reduce_set;		//����ʽ����
vector<Proj> proj_set;			//��Ŀ����
vector<char> Terminator;		//�ս��
vector<char> NonTerminator;		//���ս��
map<char, vector<char>> NonT_Follow;		//�����ս����follow��
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

//��ʼ���������ķ��Ļ�����Ϣ
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

	char ter[ter_num] = { '+', '-', '*', '/', '(', ')', 'n', '$' };	//�ս��
	char nonter[nonter_num] = { 'S', 'E', 'T', 'F' };				//���ս��

	for (int i = 0; i < ter_num; i++)
		Terminator.push_back(ter[i]);
	for (int i = 0; i < nonter_num; i++)
		NonTerminator.push_back(nonter[i]);

	//���ս����Follow��
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

//����������Ŀ
void Produce_Proj(void)
{
	vector<Reduce>::iterator it, begin;
	begin = reduce_set.begin();

	//�������в���ʽ
	for (it = reduce_set.begin(); it != reduce_set.end(); it++)
	{
		//��������Գ����ڲ���ʽ�е�λ��
		for (int i = 0; i < (*it).right.length(); i++)
		{
			Proj temp;
			temp.pID = proj_num;
			temp.rID = it - begin;
			temp.dot_pos = i;
			proj_set.push_back(temp);
			proj_num++;
		}

		//����Գ����ڲ���ʽ��󣬼���Լ��Ŀ
		Proj temp;
		temp.pID = proj_num;
		temp.rID = it - begin;
		temp.dot_pos = dot_end;
		proj_set.push_back(temp);
		proj_num++;
	}
}