#include"main.h"

char buffer[max_input] = { '\0' };			//输入缓冲区
bool print = false;							//是否打印更多分析过程

void Getarg(int argc, char* argv[])
{
	if (argc == 2)
	{
		char* input = argv[1];
		if (strcmp(input, "-d") == 0)
			print = true;
	}
}

void get_input(void)
{
	cout << "请输入算数表达式：" << endl;
	cin >> buffer;
	buffer[strlen(buffer)] = '$';
}

int main(int argc, char* argv[])
{
	Getarg(argc, argv);		//处理用户输入
	init();					//初始化文法信息
	CreateProjSet();
	PrintProjs();
	create_ana_table();		//构造分析表
	print_ana_table();		//打印分析表
	//get_input();			//获取用户输入的字符串
	//Grammar_ana();			//对字符串进行语法分析

	return 0;
}

