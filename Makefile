compile:
	gcc account.c main.c -o bank

run: compile
	./bank

compile-debug:
	gcc -g account.c main.c -o bank
