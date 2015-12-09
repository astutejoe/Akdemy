CC = gcc
CFLAGS = -std=c99 -lm
OBJ = lex.o main.o table.o
EXEC = akc

all: clean
	$(CC) $(CFLAGS) -c *.c
	gcc $(CFLAGS) -o $(EXEC) $(OBJ)

debug: clean
	$(CC) -D DEBUG $(CFLAGS) -c *.c
	gcc $(CFLAGS) -D DEBUG -o $(EXEC) $(OBJ)

run:
	./$(EXEC) ../examples/fibonacci.ak out.asm
	nasm -felf64 out.asm
	ld out.o -o out

clean:
	touch out	
	touch x.o
	touch $(EXEC)
	touch x.exe
	rm out
	rm *.o
	rm $(EXEC)
	rm *.exe
