#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;

#define ter_num 8
#define nonter_num 5

typedef struct reduce {
	char left;
	string right;
	vector<char>first;
}Reduce;



void init(void);
void create_ana_table(void);