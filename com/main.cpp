#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include "LZSS.h"
#include "Huffman.h"
using namespace std;

int main(int argc, char** argv){

	char *filename = (char*)malloc(sizeof(char)*100);
	if(argc < 2){
		cout << "input file: ";
		cin >> filename;
	}else{
		filename = argv[1];
	}
	ifstream fp;
	fp.open( filename, ios::in | ios::binary );
	if(!fp){
		cout << "ファイル'" << filename << "'が開けません" << endl;
		return 1;
	}
	unsigned char c;
	vector<unsigned char> file;
	vector<unsigned char> comp1;
	vector<unsigned char> comp2;
	vector<unsigned char> decomp1;
	while(!fp.eof()){
		fp.read((char*)&c, sizeof(unsigned char));
		//cout << (int)c << endl;
		file.push_back(c);
	}
	fp.close();

	
	cout << "before" << endl;
	for(int i=0;i<file.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)file[i] << endl;
	}
	LZSS::LZSS_compress(file, comp1);
	
	cout << "after1" << endl;
	for(int i=0;i<comp1.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)comp1[i] << endl;
	}

	Huff::Huff_compress(comp1,comp2);
	//LZSS::LZSS_decompress(comp1,decomp1);

	
	cout << "after2" << endl;
	for(int i=0;i<comp2.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)comp1[i] << endl;
	}
	
	//free
	vector<unsigned char>().swap(file);
	vector<unsigned char>().swap(comp1);

	return 0;
}
