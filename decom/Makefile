CPP=g++
FLAG=-O3 -std=c++17

all: main

main: main.o
	$(CPP) -o  main  main.o

main.o: main.cpp
	$(CPP) -c $(FLAG) main.cpp 

clean:; rm -f *.o *~main
