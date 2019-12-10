compile:
	gcc client.c account.c main.c -o bank

run: compile
	./bank

compile-debug:
	gcc -g client.c account.c main.c -o bank
