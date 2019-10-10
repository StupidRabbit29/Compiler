#include"main.h"
using namespace std;

extern char buf[half_buf_size * 2];
int totalnum = 0;


//从文件中读取缓存区的一半长度的字符
void getinput(ifstream &inprog, int pos)
{
	/*inprog.seekg(0, fstream::end);
	cout << inprog.tellg() << EOF << endl;*/
	char temp[half_buf_size + 1] = { '\0' };

	inprog.read(/*buf + pos * half_buf_size*/temp, half_buf_size - 1);
	int num = strlen(temp);
	totalnum += num;
	temp[num] = -1;
	//cout << inprog.tellg();//	EOF = -1
	strcpy_s(buf + pos * half_buf_size, sizeof(temp), temp);
	buf[half_buf_size - 1 + pos * half_buf_size] = -1;
}