all:
	gcc -fPIC people.c -c
	gcc -shared people.o -o libmylib.so
	gcc main.c people.c -o a -L. -lmylib -Wl,-rpath=.

