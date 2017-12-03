CFLAGS = -Wall -Werror -Wextra

run-test: test
	prove -v ./test

test: test.o picotest.o
	$(CXX) -o test test.o picotest.o ../libcompression_gzip.a -lz

test.o: test.cpp
	$(CXX) -Wall -Werror -Wextra -c test.cpp

picotest.o: picotest.c
	$(CC) -c picotest.c
