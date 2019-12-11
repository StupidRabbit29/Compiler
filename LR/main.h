#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<stack>
#include<string>
#include<cstring>
using namespace std;

#define ter_num 8
#define nonter_num 4
#define max_input 1024

#define dot_end -1

//产生式
typedef struct reduce {
	int rID;				//产生式的唯一标号
	char left;				//产生式左部
	string right;			//产生式右部
	//vector<char>first;	//产生式first集
}Reduce;

typedef struct proj {
	int pID;				//项目的唯一标号
	int rID;				//产生式的唯一标号
	int dot_pos;			//产生式中插入点的位置
}Proj;

typedef struct pprojS {
	int sID;				//项目集的唯一标号
	char word;				//项目集间的转移字符
}PprojS;

typedef struct projS {
	int sID;				//项目集的唯一标号
	vector<int> projs;		//项目的集合
	vector<PprojS> ptr;		//指向其他项目集的指针
}ProjS;




void init(void);
void Produce_Proj(void);
void CreateProjSet(void);
void Closure(vector<int> pID, vector<int>& projs);
void PrintProjs(void);
bool checkSameProjS(ProjS& a, vector<ProjS>& exist);
int findsID(ProjS& a, vector<ProjS>& exist);
bool checkSameProj(ProjS& a, ProjS& b);

void findreduce(char left, vector<int>& temp);
int findproj(int rID, int dot_pos);
bool Projexit(int pID, vector<int>temp);
bool BeTer(char a);
bool BeNonTer(char a);
