
all:
	gcc -Wall -std=c99 -pedantic idispatcher.c -o idispatcher
clean:
	rm test

valgrind:
	valgrind --leak-check=full --track-origins=yes ./idispatcher <test_inputs/test0.in
