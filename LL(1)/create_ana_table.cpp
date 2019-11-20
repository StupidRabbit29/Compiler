#include"main.h"

extern vector<Reduce> reduce_set;		//����ʽ����
extern vector<char> Terminator;			//�ս��
extern vector<char> NonTerminator;		//���ս��
extern map<char, vector<char>> NonT_Follow;		//�����ս����follow��
extern Reduce ana_table[nonter_num][ter_num];	//Ԥ�������
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;
extern Reduce blank;

void create_ana_table(void)
{
	//Ԥ�����Ƚ�������������λ�ö����Ͽհ�
	for (int i = 0; i < nonter_num; i++)
		for (int j = 0; j < ter_num; j++)
			ana_table[i][j] = blank;

	//�������в���ʽ	A -> ......
	vector<Reduce>::iterator it;
	for (it = reduce_set.begin(); it != reduce_set.end(); it++)
	{
		//���������ս��
		vector<char>::iterator a;
		for (a = Terminator.begin(); a != Terminator.end(); a++)
		{
			//�жϸ��ս���Ƿ��ڲ���ʽ�Ҳ���first����
			vector<char>::iterator result = find((*it).first.begin(), (*it).first.end(), *a);

			//���ڣ�������ʽ���뵽�������Ӧλ����
			if (result != (*it).first.end())
				ana_table[NonTer_to_num[(*it).left]][Ter_to_num[*a]] = *it;
		}

		//�жϲ���ʽ�Ҳ���first��������epsilon
		vector<char>::iterator result = find((*it).first.begin(), (*it).first.end(), 'e');
		//��epsilon���ڸò���ʽ�Ҳ���first��
		if (result != (*it).first.end())
			//���ò���ʽ����A��follow����Ԫ�ض�Ӧ��λ��
		{
			vector<char>::iterator b;
			for (b = NonT_Follow[(*it).left].begin(); b != NonT_Follow[(*it).left].end(); b++)
				ana_table[NonTer_to_num[(*it).left]][Ter_to_num[*b]] = *it;
		}
	}
}

void print_ana_table(void)
{
	for (int i = 0; i < nonter_num; i++)
	{
		for (int j = 0; j < ter_num; j++)
		{
			if (ana_table[i][j].right == "blank")
				cout << "Blank" << '\t';
			else
				cout << ana_table[i][j].left << " -> " << ana_table[i][j].right << '\t';
		}
		cout << '\n';
	}
}