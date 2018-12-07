#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include "libs.h"
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
	while(!fp.eof()){
		fp.read((char*)&c, sizeof(unsigned char));
		//cout << (int)c << endl;
		file.push_back(c);
	}
	fp.close();

	//ここから圧縮(LZ77)
	ring_buffer m_ring(windowsize);
	auto file_pointer = file.begin();
	int file_size = file.size();		
	while(file_size > 0){
		int head = 0;
		int len = 0;
		//圧縮成功
		if(m_ring.matching_cycle(file_pointer,file_size-1,&head, &len)){
			unsigned char s1 = 0;
			unsigned char s2 = 0;
			compress(m_ring.Iscov()-head-1, len, &s1, &s2);
			comp1.push_back(s1);
			comp1.push_back(s2);
			for(auto j=0;j<len;j++){
				m_ring.push(*file_pointer);
				file_pointer++;
				file_size--;
			}
		//失敗
		}else{
			comp1.push_back(*file_pointer);
			m_ring.push(*file_pointer);
			file_pointer++;
			file_size--;
		}
	}
	cout << "before" << endl;
	for(int i=0;i<file.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)file[i] << endl;
	}
	cout << "after" << endl;
	for(int i=0;i<comp1.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)comp1[i] << endl;
	}

	//free
	vector<unsigned char>().swap(file);
	vector<unsigned char>().swap(comp1);

	return 0;
}
