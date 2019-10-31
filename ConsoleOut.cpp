#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) noexcept{
	using namespace std;
	if (argc <= 1) return -1;
	fstream fin(argv[1]);
	while (!fin.eof()){
		cout.put(fin.get());
	}
	fin.close();
	return 0;
}