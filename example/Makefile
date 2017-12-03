all: example-static example-shared

example-static: example.o ../libcompression_gzip.a
	$(CXX) -o example-static example.o ../libcompression_gzip.a -lz

example-shared: example.o ../libcompression_gzip.so
	$(CXX) -o example-shared example.o -L$(PWD)/.. -lcompression_gzip -Wl,-rpath,$(PWD)/..

example.o: example.cpp
	$(CXX) -fPIC -Wall -Werror -Wextra -I.. -c example.cpp
