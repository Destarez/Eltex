all:
	gcc -fPIC -c list.c -o list.o
	gcc -shared list.o -o liblist.so
	gcc -o main main.c -L. -llist -ldl
	LD_LIBRARY_PATH=/home/destare/calculator/"Module 2"/6.2 ./main



