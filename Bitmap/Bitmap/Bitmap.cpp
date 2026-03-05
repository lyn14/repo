#include <iostream>
#include <cmath>

using namespace std;

class Bitmap {
private:
	char* M; int N; //M是存放位图的数组，N代表M的容量，于是总的比特数为N*8；
public:
	Bitmap() { N = 0; M = new char; M = 0; }
	Bitmap(int n) { N = (n + 7) / 8; M = new char[N]; for (int i = 0; i < N; i++) { M[i] = 0; } }
	int insert(int i);
	bool search(int& memb);
	void remove(int& memb);
	void expand(int i);
	~Bitmap() { delete M; }
};

//以下均默认M数组是“从左往右”生长的；

int Bitmap::insert(int i) {
	expand(i); 
	M[i >> 3] &= (0x80 >> (i % 8) ) ;
	return i;
}

bool Bitmap::search(int& memb) {
	return M[ memb >> 3] & (0x80 >> (memb % 8));
}

void Bitmap ::remove(int& memb) {
	if (!M[memb >> 3]) return;
	M[ memb >> 3] |= ~( 0x80 >> (memb % 8) );
}

void Bitmap ::expand(int i) {
	char* oldM = M;
	int oldN = N;
	if (i >= 8 * N) { N = 2 * i; Bitmap N; for (int j = 0; j < oldN; j++) { oldM[j] = M[j]; } }
	return;
}