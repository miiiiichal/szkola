all: main
#ustawienie kompilatora dla pliku
CC=g++
#alternatywnie w CC mozna podać ścieżkę do crosscompiler'a

functions.o: functions.cpp
#	gcc -c -o functions.o functions.cpp
	$(CC) -c -o functions.o functions.cpp
main.o: main.cpp
#	gcc -c -o main.o main.c
	$(CC) -c -o main.o main.cpp

main: main.o functions.o
#	gcc -o main main.o functions.o
	$(CC) -o main main.o functions.o

