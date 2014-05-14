 # See README.txt.

.PHONY: all cpp clean

all: cpp

cpp:    test

clean:
	rm -f test

test: src/testmain.cpp
	pkg-config --cflags protobuf  # fails if protobuf is not installed
	c++ src/testmain.cpp -o test `pkg-config --cflags --libs protobuf`
