#include"main.h"

char buffer[max_input] = { '\0' };			//���뻺����
bool print = false;							//�Ƿ��ӡ�����������

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
	cout << "�������������ʽ��" << endl;
	cin >> buffer;
	buffer[strlen(buffer)] = '$';
}

int main(int argc, char* argv[])
{
	Getarg(argc, argv);		//�����û�����
	init();					//��ʼ���ķ���Ϣ
	CreateProjSet();
	PrintProjs();
	create_ana_table();		//���������
	print_ana_table();		//��ӡ������
	//get_input();			//��ȡ�û�������ַ���
	//Grammar_ana();			//���ַ��������﷨����

	return 0;
}

