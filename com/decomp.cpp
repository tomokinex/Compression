#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include "LZSS.h"
#include "Huffman.h"
using namespace std;

int main(int argc, char** argv){

	char *infilename = (char*)malloc(sizeof(char)*100);
	char *outfilename = (char*)malloc(sizeof(char)*100);

	if(argc < 2){
		cout << "input file: ";
		cin >> infilename;
	}else{
		infilename = argv[1];
	}
	if(argc < 3){
		cout << "output file: ";
		cin >> outfilename;
	}else{
		outfilename = argv[2];
	}

	ifstream infp;
	infp.open( infilename, ios::in | ios::binary );
	if(!infp){
		cout << "ファイル'" << infilename << "'が開けません" << endl;
		return 1;
	}
	unsigned char c;
	vector<unsigned char> file;
	vector<unsigned char> decomp1;
	vector<unsigned char> decomp2;

	int byte_counter_in = 0;
	int byte_counter_out = 0;
	
	while(!infp.eof()){
		infp.read((char*)&c, sizeof(unsigned char));
		file.push_back(c);
	}
	infp.close();
	byte_counter_in += file.size();
	cout << infilename << " size: " << byte_counter_in << "byte" << endl;
	/*
	cout << "before" << endl;
	for(int i=0;i<file.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)file[i] << endl;
	}
	*/
	Huff::Huff_decompress(file,decomp1);

	cout << "after Huffman size: " << decomp1.size() << "byte" << endl;
	/*
	cout << "after1" << endl;
	for(int i=0;i<comp1.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)comp1[i] << endl;
	}
	*/
	LZSS::LZSS_decompress(decomp1, decomp2);

	cout << "after LZSS size: " << decomp2.size() << "byte" << endl;

	/*
	cout << "after2" << endl;
	for(int i=0;i<comp2.size();i++){
		cout << i << " : ";
		cout << (int)comp2[i] << endl;
	}
	*/
	std::ofstream  outfp( outfilename, std::ios::binary );

	for(int i=0;i<decomp2.size();i++){
		outfp.write( reinterpret_cast<char*>(&decomp2[i]), sizeof(unsigned char) );
	}
	byte_counter_out += decomp2.size();
	
	outfp.close();
	//free

	vector<unsigned char>().swap(file);
	vector<unsigned char>().swap(decomp1);
	vector<unsigned char>().swap(decomp2);

	return 0;
}
