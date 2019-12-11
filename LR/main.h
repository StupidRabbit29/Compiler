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

//����ʽ
typedef struct reduce {
	int rID;				//����ʽ��Ψһ���
	char left;				//����ʽ��
	string right;			//����ʽ�Ҳ�
	//vector<char>first;	//����ʽfirst��
}Reduce;

typedef struct proj {
	int pID;				//��Ŀ��Ψһ���
	int rID;				//����ʽ��Ψһ���
	int dot_pos;			//����ʽ�в�����λ��
}Proj;

typedef struct pprojS {
	int sID;				//��Ŀ����Ψһ���
	char word;				//��Ŀ�����ת���ַ�
}PprojS;

typedef struct projS {
	int sID;				//��Ŀ����Ψһ���
	vector<int> projs;		//��Ŀ�ļ���
	vector<PprojS> ptr;		//ָ��������Ŀ����ָ��
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
