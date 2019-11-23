#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<stack>
#include<string>
#include<cstring>
using namespace std;

#define ter_num 8
#define nonter_num 5
#define max_input 1024

//产生式
typedef struct reduce {
	char left;				//产生式左部
	string right;			//产生式右部
	vector<char>first;		//产生式first集
}Reduce;



void init(void);
void create_ana_table(void);
void print_ana_table(void);
void Grammar_ana(void);
void get_input(void);
bool BeNonTer(char a);
bool BeTer(char a);
bool digit(char a);
void ErrorControl(char ter, char nont);
void print_stack(void);