all: helper.o main.o functionsToassembly.o assemblerOperations.o
	gcc -g -Wall -o main main.o helper.o functionsToassembly.o assemblerOperations.o
main.o: main.c main.h
	gcc -g -Wall -c -o main.o main.c
helper.o:  helper.c main.h
	gcc -g -Wall -c -o helper.o helper.c
#functionsToassembly.o: functionsToassembly.c main.h
#	gcc -g -Wall -c -o functionsToassembly.o functionsToassembly.c
assemblerOperations.o: assemblerOperations.s
	nasm -f elf64 assemblerOperations.s -l asm.list
	nasm -g -f elf64 -w+all -o assemblerOperations.o assemblerOperations.s
.PHONY: clean
clean:
	rm -f *.o main
