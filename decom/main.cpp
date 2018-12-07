#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;
int main(){
	string filename;
	cout << "input file: ";
	cin >> filename;
	ifstream fp;
	fp.open( filename, ios::in | ios::binary );
	if(!fp){
		cout << "ファイル'" << filename << "'が開けません" << endl;
		return 1;
	}
	char c;
	while(!fp.eof()){
		fp.read((char*)&c, sizeof(char));
		cout << (int)c << endl;
	}
	fp.close();
	return 0;
}
