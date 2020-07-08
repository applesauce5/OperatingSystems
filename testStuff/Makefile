CC = gcc
CFLAGS = -g

all: test

test:
	$(CC) test.c -lpthread -o test
pete:
	gcc -pthread peterson_spinlock.c -o peterson_spinlock
clean:
	rm -rf test
