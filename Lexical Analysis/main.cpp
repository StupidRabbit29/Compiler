#include"main.h"
using namespace std;

char defaultfile[] = "D://VS_Projects//Compiler//Debug//test.c";
char* filename = defaultfile;

char buf[half_buf_size * 2];
char* lexemebegin;
char* forwardp;
char token[half_buf_size];



//��ȡ�������ļ��ļ���
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
	//��ȡ�������ļ��ļ���
	if (Getarg(argc, argv))
	{
		//���ļ�
		ifstream inprog(filename, ifstream::in | ifstream::binary);
	
		//���дʷ���������
		Status result = analysis(inprog);


	}
	else
		cout << "����ĳ�ʼ����������" << endl;

	return 0;
}