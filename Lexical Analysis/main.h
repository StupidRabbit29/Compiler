#pragma once
#include<iostream>
#include<fstream>
using namespace std;

#define half_buf_size 1024
#define buf_left 0
#define buf_right 1

enum Status{OK, ERR, TER};


void getinput(ifstream& inprog, int pos);
Status analysis(ifstream& inprog);
char get_char(ifstream& inprog);
void get_npc(ifstream& inprog, char& C);
bool letter(char& C);
bool digit(char& C);


