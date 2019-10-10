#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
using namespace std;

#define half_buf_size 1024
#define buf_left 0
#define buf_right 1

#define keynum 32
#define optnum 16
#define endnum 13
#define othernum 12

#define WORD 1				//��ͨ��ʶ��
#define KEYWORD 2			//�ؼ���
#define INTDIG 3			//����
#define FLOATDIG 4			//������
#define OPTION 5			//��������
#define END 6				//���
#define OTHER 7				//��������

typedef struct token
{
	int type;			//�Ǻ�
	int index;			//����
	int line;			//��������
}Token;

enum Status{OK, ERR, TER};
enum ErrorType{Wrong_dot, Wrong_neg, Wrong_E, Invalid_character, Other_wrong};


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


