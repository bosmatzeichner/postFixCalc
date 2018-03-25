all: main.o
	gcc -g -Wall -o main main.o
main.o: main.c
	gcc -g -Wall -c -o main.o main.c
clean: 
	rm main.o main
