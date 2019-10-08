#include"main.h"
using namespace std;

char defaultfile[] = "D://VS_Projects//Compiler//Debug//test.c";
char* filename = defaultfile;

char buf[half_buf_size * 2];
char* lexemebegin;
char* forwardp;
char token[half_buf_size];



//获取被编译文件文件名
bool Getarg(int argc, char* argv[])
{
	if (argc == 2)
	{
		filename = argv[1];
		return true;
	}
	else if (argc > 2)
		return false;
}

int main(int argc, char* argv[])
{
	//获取被编译文件文件名
	if (Getarg(argc, argv))
	{
		//打开文件
		ifstream inprog(filename, ifstream::in | ifstream::binary);
	
		//运行词法分析程序
		Status result = analysis(inprog);


	}
	else
		cout << "输入的初始化参数错误！" << endl;

	return 0;
}