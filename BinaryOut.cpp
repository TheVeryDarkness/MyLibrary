#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char *argv[]) noexcept{
	using namespace std;
	if (argc <= 1)
		return cerr << "No file is provided in argument." << endl, -1;
	for (int i = 1; i < argc; ++i){
		ifstream fin(argv[i], ios_base::binary);
		if (!fin){
			cerr << "Error in opening the file at \"" << argv[i] << "\"." << endl;
			continue;
		}
		cout << "/*Below is " << argv[i] << " */" << endl;
		cout << setfill('0') << setbase(16);
		size_t bytes = 0;
		while (fin.peek(), !fin.eof()){
			if (bytes % 16 == 15)
				cout << "\b: " << bytes - 15 << "H - " << bytes << 'H' << endl ;
			cout << setw(2) << static_cast<unsigned int>(fin.get()) << ' ';
			++bytes;
			}
		cout << endl;
		fin.close();
	}
	return 0;
}