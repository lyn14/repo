#include <iostream>
#include <cmath>

using namespace std;

class Bitmap {
private:
	char* M; int N; //M是存放位图的数组，N代表M的容量
public:
	Bitmap() { N = 0; M = new char; M = 0; }
	Bitmap(int n) { N = (n + 7) / 8; M = new char[N]; for (int i = 0; i < N; i++) { M[i] = 0; } }
	int insert(int i);
	bool search(int& memb);
	void remove(int& memb);
	~Bitmap() { delete M; }
};

int Bitmap::insert(int i) {
	expand(i); 
	M[i >> 3] &= 0x80 >> i % 0x70;
	return i;
}

bool Bitmap::search(int& memb) {
	return M[memb >> 3];
}

void Bitmap ::remove(int& memb) {
	if (!M[memb >> 3]) return;
	M[memb >> 3] |~ = (0x80 >> memb % 0x70 );
}