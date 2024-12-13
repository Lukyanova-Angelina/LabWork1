all: prog
	
prog: *.o
	g++ *.o -o prog

*.o: *.cpp
	g++ -c *.cpp


clean:
	rm -rf *.o prog

