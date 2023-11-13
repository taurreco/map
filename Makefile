
CC = gcc

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -g

all: test

test:
	$(CC) -g test.c unity/unity.c -Iunity -I. -o test

check: test
	./test

clean:
	rm test
