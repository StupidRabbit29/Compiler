#include"main.h"

extern vector<Reduce> reduce_set;		//����ʽ����
extern vector<Proj> proj_set;			//��Ŀ����
extern vector<char> Terminator;		//�ս��
extern vector<char> NonTerminator;		//���ս��
extern map<char, vector<char>> NonT_Follow;		//�����ս����follow��
extern Reduce blank;
extern Reduce synch;
extern map<char, int> Ter_to_num;
extern map<char, int> NonTer_to_num;

extern int proj_num = 0;

void CreateProjSet(void)
{

}

//����������ɲ���ʽ���ϵıհ�
void Closure(vector<int> pID, vector<int>& projs)
{
	vector<int> temp;
	//�����������ɲ���ʽ���ȼ���temp
	for (vector<int>::iterator it = pID.begin(); it != pID.end(); it++)
		temp.push_back(*it);

	//��ʼ�����ж���ĳ�����ս����Ϊ����ʽ�󲿵����в���ʽ�Ƿ��Ѿ�����temp
	bool nonteradd[nonter_num] = { false };

	while (true)
	{
		//�����Ƿ��ҵ�ĳ����Ŀ�еĵ�����Ƿ��ս��
		bool findnonter = false;

		//�����������еĲ���ʽ
		for (vector<int>::iterator it = temp.begin(); it != temp.end(); it++)
		{
			//����ʽ�Ļ�����Ϣ
			int rID = proj_set.at((*it)).rID;
			int dot = proj_set.at((*it)).dot_pos;
			if (dot == dot_end)
				continue;
			char ch = reduce_set.at(rID).right[dot];
			//�жϲ���ʽ�е�ĺ����Ƿ��Ƿ��ս����������Ϊ�󲿵����в���ʽ�Ƿ��Ѿ�����temp
			if (BeNonTer(ch) && nonteradd[NonTer_to_num[ch]] == false)
			{
				findnonter = true;
				//�ҵ������Ը��ս��Ϊ�󲿵Ĳ���ʽ
				vector<int>ch_left;
				findreduce(ch, ch_left);
				//����Щ����ʽ���ҵ��ڿ�ʼλ�õ���Ŀ����temp
				for (vector<int>::iterator it = ch_left.begin(); it != ch_left.end(); it++)
				{
					int pID = findproj(*it, 0);
					if (!Projexit(pID, temp))
						temp.push_back(pID);
				}
			}
		}

		//����Ѿ�û�и��£����˳�
		if (findnonter == false)
			break;
	}
	projs = temp;
}

//�ж��ַ��Ƿ��Ƿ��ս��
bool BeNonTer(char a)
{
	vector<char>::iterator it;
	it = find(NonTerminator.begin(), NonTerminator.end(), a);
	if (it == NonTerminator.end())
		return false;
	else
		return true;
}

void findreduce(char left, vector<int>& temp)
{
	for (vector<Reduce>::iterator it = reduce_set.begin(); it != reduce_set.end(); it++)
		if (left == (*it).left)
			temp.push_back((*it).rID);
}

int findproj(int rID, int dot_pos)
{
	for(vector<Proj>::iterator it = proj_set.begin() ; it != proj_set.end() ; it++)
		if (rID == (*it).rID && dot_pos == (*it).dot_pos)
			return (*it).pID;
}

bool Projexit(int pID, vector<int>temp)
{
	vector<int>::iterator it;
	it = find(temp.begin(), temp.end(), pID);
	if (it == temp.end())
		return false;
	else
		return true;
}



