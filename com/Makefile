CPP=g++
FLAG=-O3 -std=c++11 -lm -g

all: comp decomp

comp: main.o
	$(CPP) -o  comp main.o

decomp: decomp.o
	$(CPP) -o decomp decomp.o

decomp.o: decomp.cpp LZSS.h Huffman.h defines.h
	$(CPP) -c $(FLAG) decomp.cpp 

main.o: main.cpp LZSS.h Huffman.h defines.h
	$(CPP) -c $(FLAG) main.cpp 

clean:; rm -f *.o *~comp
