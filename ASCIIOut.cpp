#include <iostream>
#include <fstream>
#include <iomanip>

template<bool _signed> class helper;

template<>class helper<true>{
	public: using int_type = int;
};
template<>class helper<false>{
	public: using int_type = unsigned int;
};
using target_int = helper<static_cast<char>(-1) < 0>::int_type;
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
			if (c < 32 || c >= 127)
				cout << "\\x" << setw(2) << static_cast<target_int>(static_cast<unsigned char>(c));
			else cout << c;
		}
		cout << endl;
		fin.close();
	}
	return 0;
}