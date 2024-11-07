all: main.o
	g++ main.o -o mf
main.o: main.cpp
	g++ -c main.cpp
clean:
	rm -rf *.o prog
