findcomp: main.o
	g++ -std=c++20 main.o -o findcomp

main.o: main.cpp
	g++ -std=c++20 -c main.cpp -o main.o

clean:
	rm *.o findcomp