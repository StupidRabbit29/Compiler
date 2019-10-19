#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
using namespace std;

#define half_buf_size 1024	//单缓存区长度
#define buf_left 0			//左侧缓存区
#define buf_right 1			//右侧缓存区

#define keynum 32			//关键字数量
#define optnum 16			//操作符数量
#define endnum 13			//界符数量
#define othernum 12			//其他符号数量

#define WORD 1				//普通标识符
#define KEYWORD 2			//关键字
#define INTDIG 3			//整数
#define FLOATDIG 4			//浮点数
#define OPTION 5			//操作符号
#define END 6				//界符
#define OTHER 7				//其他符号

//Token 设置
typedef struct token
{
	int type;			//记号
	int index;			//属性
	int line;			//所在行数
}Token;

enum Status{OK, ERR, TER};
enum ErrorType{Wrong_dot, Wrong_neg, Wrong_E, Invalid_character, Other_wrong, Wrong_identifier};


void getinput(ifstream& inprog, int pos);
Status analysis(ifstream& inprog);
char get_char(ifstream& inprog);
void get_npc(ifstream& inprog, char& C);
bool letter(char& C);
bool digit(char& C);
int reserve(char* token);
void cat(char& C, int& cur);
void retract(void);
long long get_int(char* buf);
int get_pnum_length(long long temp);
double get_float(void);
void add(int type, int num);
void error(ErrorType type);


