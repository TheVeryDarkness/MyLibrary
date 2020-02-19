#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char* argv[]) noexcept{
	using namespace std;
	if (argc <= 1) return cerr << "No file is provided in argument." << endl, -1;
	for(int i = 1; i < argc; ++i){
		ifstream fin(argv[i]);
		if(!fin) {
			cerr << "Error in opening the file at \"" << argv[i] <<"\"." << endl;
			continue;
		}
		cout << "/*Below is " << argv[i] << " */" << endl << setfill('0') <<setbase(16);
		while (fin.peek(), fin){
			char c = fin.get();
			if ((0 <= c && c < 32) || c == 127)
				cout << "\\x" << setw(3) << static_cast<unsigned>(c);
			else cout << c;
		}
		cout << endl;
		fin.close();
	}
	return 0;
}