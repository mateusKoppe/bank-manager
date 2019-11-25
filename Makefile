compile:
	gcc main.c account.c -o bank

run: compile
	./bank

compile-debug:
	gcc -g main.c account.c -o bank
