all: cat inject lib.so

cat: cat.c
	gcc $^ -o $@

lib.so: lib.c
	gcc -fPIC -shared -ldl $^ -o $@

inject: inject.c
	gcc $^ -o $@

test:
	./inject ./cat

clean:
	rm ./cat ./inject ./lib.so

.PHONY: clean

