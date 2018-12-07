#include<iostream>
#include<fstream>
#include<string.h>
#include<vector>
#include "libs.h"
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
	vector<char> file;
	vector<char> comp;
	while(!fp.eof()){
		fp.read((char*)&c, sizeof(char));
		cout << (int)c << endl;
		file.push_back(c);
	}
	fp.close();

	ring_buffer m_ring(windowsize);
	auto file_pointer = file.begin();		
	for(auto i=0;i<file.size() && file_pointer != file.end();i++){
		int head = 0;
		int len = 0;
			//圧縮成功
		if(m_ring.matching_cycle(file_pointer,&head, &len)){
			char s1 = 0;
			char s2 = 0;
			compress(head, len, &s1, &s2);
			comp.push_back(s1);
			comp.push_back(s2);
			for(auto j=0;j<len;j++){
				m_ring.push(*file_pointer);
				file_pointer++;
			}
			//失敗
		}else{
			comp.push_back(*file_pointer);
			m_ring.push(*file_pointer);
			file_pointer++;
		}
	}
	cout << "before" << endl;
	for(int i=0;i<file.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)file[i] << endl;
	}
	cout << "after" << endl;
	for(int i=0;i<comp.size();i++){
		cout << i << " : ";
		cout << hex << showbase << (int)comp[i] << endl;
	}
	return 0;
}
