
build/bfi: lib/bfi.o lib/test.o
	cc -g -Wall lib/test.o lib/bfi.o -o build/bfi
	
lib/bfi.o: src/bfi.c
	cc -g -Wall -c src/bfi.c -o lib/bfi.o
	
lib/test.o: src/bfi.h src/test.c
	cc -g -Wall -c src/test.c -o lib/test.o
	
clean:
	rm -f build/*
	rm -f lib/*
	rm -rf *.dSYM
	
run: build/bfi
	./build/bfi
	