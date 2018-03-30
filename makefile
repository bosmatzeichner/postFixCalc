all: helper.o main.o
	gcc -g -Wall -o main main.o helper.o
main.o: main.c
	gcc -g -Wall -c -o main.o main.c
helper.o:  helper.c main.h
	gcc -g -Wall -c -o helper.o helper.c
clean:
	rm main.o main helper.o
