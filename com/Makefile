CPP=g++
FLAG=-O3 -std=c++11 -lm -g

all: comp

comp: main.o
	$(CPP) -o  comp main.o

main.o: main.cpp LZSS.h Huffman.h defines.h
	$(CPP) -c $(FLAG) main.cpp 

clean:; rm -f *.o *~comp
