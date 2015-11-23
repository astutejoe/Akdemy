CC = gcc
CFLAGS = -std=c99 -D DEBUG
OBJ = lex.o main.o table.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJ)
	gcc $(CFLAGS) -o ml $^

run:
	./ml ../examples/fibonacci.l out.o

clean:
	rm *.o
	rm ml
