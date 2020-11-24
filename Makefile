build: compile
	touch storage/account.txt
	touch storage/client.txt
	touch storage/client.index-name.txt

compile:
	gcc client.c account.c main.c -o bank

clean: 
	rm storage/account.txt -f
	rm storage/client.txt -f
	rm storage/client.index-name.txt -f
	make build

run: compile
	./bank

compile-debug:
	gcc -g client.c account.c main.c -o bank
