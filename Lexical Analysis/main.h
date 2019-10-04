#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

#define half_buf_size 1024
#define buf_left 0
#define buf_right 1
#define keynum 32
#define optnum 16
#define endnum 12
#define othernum 10

#define WORD 1
#define KEYWORD 2
#define INTDIG 3
#define FLOATDIG 4
#define OPTION 5
#define END 6
#define OTHER 7

typedef struct token
{
	int type;			//记号
	int index;			//属性
	int line;			//所在行数
}Token;

enum Status{OK, ERR, TER};


void getinput(ifstream& inprog, int pos);
Status analysis(ifstream& inprog);
char get_char(ifstream& inprog);
void get_npc(ifstream& inprog, char& C);
bool letter(char& C);
bool digit(char& C);
int reserve(char* token);
void cat(char& C, int& cur);
void retract(void);
long long get_int(void);
void add(int type, int num);


