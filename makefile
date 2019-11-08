CC = gcc
CFLAGS = -Wall -g

all: dict1 dict2

dict1: main.c dict1.o
	$(CC) $(CFLAGS) main.c dict1.o -o dict1
dict2: main.c dict2.o
	$(CC) $(CFLAGS) main.c dict2.o -o dict2
dict1.o: dict1.c
	$(CC) $(CFLAGS) -c dict1.c
dict2.o: dict2.c
	$(CC) $(CFLAGS) -c dict2.c
clean:
	rm *.o dict1 dict2