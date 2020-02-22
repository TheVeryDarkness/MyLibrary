#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char* argv[]) noexcept{
	using namespace std;
	if (argc <= 1) return cerr << "No file is provided in argument." << endl, -1;
	for(int i = 1; i < argc; ++i){
		ifstream fin(argv[i], ios_base::binary);
		if(!fin) {
			cerr << "Error in opening the file at \"" << argv[i] <<"\"." << endl;
			continue;
		}
		cout << "/*Below is " << argv[i] << " */" << setfill('0') <<setbase(16);
		size_t bytes = 0;
		while (fin.peek(), fin){
			if (bytes % 16 == 0) cout << endl;
			char c = fin.get();
			if (c >= 32 && c < 127)
				cout << c;
			else cout << '\x00c';
			++bytes;
		}
		cout << endl;
		fin.close();
	}
	return 0;
}