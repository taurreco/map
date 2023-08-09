
CC = gcc

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -g

all: test

test:
	$(CC) $(CFLAGS) test.c vec.c unity/unity.c -Iunity -I. -o test

check: test
	./test

clean:
	rm test
