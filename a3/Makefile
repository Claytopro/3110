
all:
	gcc -Wall -std=c99 -pedantic idispatcher.c -o test

clean:
	rm test

valgrind:
	valgrind --leak-check=full --track-origins=yes ./test
