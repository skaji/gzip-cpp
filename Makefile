all: libcompression_gzip.so libcompression_gzip.a example/example run-test

libcompression_gzip.so: gzip.o
	$(CXX) -shared -o libcompression_gzip.so gzip.o -lz

libcompression_gzip.a: gzip.o
	ar rcs libcompression_gzip.a gzip.o

gzip.o: gzip.cpp gzip.hpp
	$(CXX) -fPIC -Werror -Wall -Wextra -c gzip.cpp

example/example:
	cd example && $(MAKE)

run-test:
	cd tests && $(MAKE)
