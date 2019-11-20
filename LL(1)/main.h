#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<stack>
#include<string>
using namespace std;

#define ter_num 8
#define nonter_num 5
#define max_input 1024

typedef struct reduce {
	char left;
	string right;
	vector<char>first;
}Reduce;



void init(void);
void create_ana_table(void);
void print_ana_table(void);
void Grammar_ana(void);
void get_input(void);
bool BeNonTer(char a);
bool BeTer(char a);
bool digit(char a);