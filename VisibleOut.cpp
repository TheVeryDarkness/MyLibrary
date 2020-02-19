#include <iostream>
#include <fstream>

constexpr inline char to_visible(char c)noexcept{
	return ((32 <= c && c <= 126) || c & 0x80)? c: '.';
}
int main(int argc, char* argv[]) noexcept{
	using namespace std;
	if (argc <= 1) return cerr << "No file is provided in argument." << endl, -1;
	for(int i = 1; i < argc; ++i){
		ifstream fin(argv[i]);
		if(!fin) {
			cerr << "Error in opening the file at \"" << argv[i] <<"\"." << endl;
			continue;
		}
		cout << "/*Below is " << argv[i] << " */" << endl;
		while (fin.peek(), fin){
			cout.put(to_visible(fin.get()));
		}
		cout << endl;
		fin.close();
	}
	return 0;
}