all: prog
	./prog
	rm -rf *.o prog
	
prog: picture.o
	g++ picture.o -o prog
	

picture.o: picture.cpp
	g++ -c picture.cpp


clean:
	rm -rf *.o prog

