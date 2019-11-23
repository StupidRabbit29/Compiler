#include"main.h"

extern vector<Reduce> reduce_set;		//产生式集合
extern vector<char> Terminator;			//终结符
extern vector<char> NonTerminator;		//非终结符
extern map<char, vector<char>> NonT_Follow;		//各非终结符的follow集
Reduce blank;
Reduce synch;

void init(void)
{
	Reduce r1;
	r1.left = 'E';
	r1.right = "TA";
	r1.first.push_back('(');
	r1.first.push_back('n');
	reduce_set.push_back(r1);

	Reduce r2;
	r2.left = 'A';
	r2.right = "+TA";
	r2.first.push_back('+');
	reduce_set.push_back(r2);

	Reduce r3;
	r3.left = 'A';
	r3.right = "-TA";
	r3.first.push_back('-');
	reduce_set.push_back(r3);

	Reduce r4;
	r4.left = 'A';
	r4.right = "";
	r4.first.push_back('e');
	reduce_set.push_back(r4);

	Reduce r5;
	r5.left = 'T';
	r5.right = "FB";
	r5.first.push_back('(');
	r5.first.push_back('n');
	reduce_set.push_back(r5);

	Reduce r6;
	r6.left = 'B';
	r6.right = "*FB";
	r6.first.push_back('*');
	reduce_set.push_back(r6);

	Reduce r7;
	r7.left = 'B';
	r7.right = "/FB";
	r7.first.push_back('/');
	reduce_set.push_back(r7);

	Reduce r8;
	r8.left = 'B';
	r8.right = "";
	r8.first.push_back('e');
	reduce_set.push_back(r8);

	Reduce r9;
	r9.left = 'F';
	r9.right = "(E)";
	r9.first.push_back('(');
	reduce_set.push_back(r9);

	Reduce r10;
	r10.left = 'F';
	r10.right = "n";
	r10.first.push_back('n');
	reduce_set.push_back(r10);

	char ter[ter_num] = { '+', '-', '*', '/', '(', ')', 'n', '$' };	//终结符
	char nonter[nonter_num] = { 'E', 'T', 'A', 'F', 'B' };			//非终结符

	for (int i = 0; i < ter_num; i++)
		Terminator.push_back(ter[i]);
	for (int i = 0; i < nonter_num; i++)
		NonTerminator.push_back(nonter[i]);

	vector<char> E{ '$', ')' };
	vector<char> A{ '$', ')' };
	vector<char> T{ '$', ')', '+', '-' };
	vector<char> B{ '$', ')', '+', '-' };
	vector<char> F{ '$', ')', '+', '-', '*', '/' };

	NonT_Follow['E'] = E;
	NonT_Follow['A'] = A;
	NonT_Follow['T'] = T;
	NonT_Follow['B'] = B;
	NonT_Follow['F'] = F;

	blank.right = "blank";
	synch.right = "synch";
}