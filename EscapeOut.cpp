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
		cout << "/*Below is " << argv[i] << " */" << endl;
		while (fin.peek(), fin){
			char c = fin.get();
			switch(c){
				case 7: cout << "\\a"; break;
				case 8: cout << "\\b"; break;
				case 9: cout << "\\t"; break;
				case 10: cout << "\\n"; break;
				case 11: cout << "\\v"; break;
				case 12: cout << "\\f"; break;
				case 13: cout << "\\r"; break;
				default:
					if ((0 <= c && c < 32) || c == 127)
						cout << "\\x" << setw(3) << static_cast<unsigned>(c);
					else cout << c;
			}
		}
		cout << endl;
		fin.close();
	}
	return 0;
}