#include"main.h"
using namespace std;

extern char buf[half_buf_size * 2];


//从文件中读取缓存区的一半长度的字符
void getinput(ifstream &inprog, int pos)
{
	/*inprog.seekg(0, fstream::end);
	cout << inprog.tellg() << EOF << endl;*/

	inprog.read(buf + pos * half_buf_size, half_buf_size - 1);
	buf[half_buf_size - 1 + pos * half_buf_size] = -1;
}