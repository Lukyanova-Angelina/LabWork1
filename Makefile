all: try2.o
	g++ try2.o -o mf
main.o: try2.cpp
	g++ -c try2.cpp
clean:
	rm -rf *.o prog
