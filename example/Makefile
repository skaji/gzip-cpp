all: example-static example-shared

example-static: example.o ../libcompression_cpp_gzip.a
	g++ -o example-static example.o ../libcompression_cpp_gzip.a -lz

example-shared: example.o ../libcompression_cpp_gzip.so
	g++ -o example-shared example.o -L$(PWD)/.. -lcompression_cpp_gzip -Wl,-rpath,$(PWD)/..

example.o: example.cpp
	g++ -fPIC -Wall -Werror -Wextra -I.. -c example.cpp
