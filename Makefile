CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -ansi -pedantic -o proj2 *.c
EXECUTABLE = projeto

all: project2.c
	$(CC) $(CFLAGS)

clean:
	rm -f *.o $(EXECUTABLE)