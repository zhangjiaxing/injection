all: cat inject lib.so libcpp.so

cat: cat.c
	gcc $^ -o $@

lib.so: lib.c
	gcc -fPIC -shared -ldl $^ -o $@

libcpp.so: libcpp.cpp
	g++ libcpp.cpp  -I /usr/include/qt5/ -I /usr/include/qt5/QtGui/ -Wl,-O1 -fPIC -shared -ldl -o libcpp.so

inject: inject.c
	gcc $^ -o $@

test:
	./inject ./cat

clean:
	rm ./cat ./inject ./lib.so ./libcpp.so

.PHONY: clean

