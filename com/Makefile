CPP=g++
FLAG=-O3 -std=c++17 -lm -g

all: main

main: main.o
	$(CPP) -o  main  main.o

main.o: main.cpp LZSS.h Huffman.h defines.h
	$(CPP) -c $(FLAG) main.cpp 

clean:; rm -f *.o *~main
