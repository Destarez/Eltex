all:
	gcc -fPIC -c dif.c -o dif.o
	gcc -shared dif.o -o libdif.so
	gcc -fPIC -c div.c -o div.o
	gcc -shared div.o -o libdiv.so
	gcc -fPIC -c sum.c -o sum.o
	gcc -shared sum.o -o libsum.so
	gcc -fPIC -c mult.c -o mult.o
	gcc -shared mult.o -o libmult.so

	gcc -o main main.c -L. -ldif -ldiv -lsum -lmult -ldl
	LD_LIBRARY_PATH=/home/destare/calculator/"Module 2"/6.3 ./main



