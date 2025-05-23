all:
	gcc -c list.c -o list.o
	ar rcs liblist.a list.o 

	gcc main.c -o main -L. -llist
	./main