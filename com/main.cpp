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
	vector<unsigned char> comp1;
	vector<unsigned char> comp2;

	int byte_counter_in = 0;
	int byte_counter_out = 0;
	
	while(!infp.eof()){
		infp.read((char*)&c, sizeof(unsigned char));
		//cout << (int)c << endl;
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
	LZSS::LZSS_compress(file, comp1);
	cout << "after LZSS size: " << comp1.size() << "byte" << endl;
	/*
	cout << "after1" << endl;
	for(int i=0;i<comp1.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)comp1[i] << endl;
	}
	*/
	Huff::Huff_compress(comp1,comp2);

	cout << "after Huffman size: " << comp2.size() << "byte" << endl;
	/*
	cout << "after2" << endl;
	for(int i=0;i<comp2.size();i++){
		cout << i << " : ";
		cout << (int)comp2[i] << endl;
	}
	*/
	std::ofstream  outfp( outfilename, std::ios::binary );

	for(int i=0;i<comp2.size();i++){
		outfp.write( reinterpret_cast<char*>(&comp2[i]), sizeof(unsigned char) );
	}
	//cout << "ZSS + Huffman" << endl;
	byte_counter_out += comp2.size();
		
	//cout << outfilename << " size: " << byte_counter_out << " byte ("  << 100.0*byte_counter_out / byte_counter_in << "%)" << endl;


	outfp.close();
	//free

	vector<unsigned char>().swap(file);
	vector<unsigned char>().swap(comp1);
	vector<unsigned char>().swap(comp2);

	return 0;
}
