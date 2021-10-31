all: cat inject lib.so

cat: cat.c
	gcc $^ -o $@

lib.so: lib.c
	gcc -fPIC -shared -ldl -std=c11 $^ -o $@

inject: inject.c
	gcc $^ -o $@

test:
	./inject ./cat

clean:
	rm ./cat ./inject ./lib.so

.PHONY: clean

