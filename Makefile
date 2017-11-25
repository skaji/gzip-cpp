all: libcompression_cpp_gzip.so libcompression_cpp_gzip.a example/example

libcompression_cpp_gzip.so: gzip.o
	g++ -shared -o libcompression_cpp_gzip.so gzip.o -lz

libcompression_cpp_gzip.a: gzip.o
	ar rcs libcompression_cpp_gzip.a gzip.o

gzip.o: gzip.cpp gzip.hpp
	g++ -fPIC -Werror -Wall -Wextra -c gzip.cpp

example/example:
	cd example && $(MAKE)
