
objects =  util.o urls.o

libs = `curl-config --libs --cflags`

CXXFLAGS = -O2 $(libs)

all:app unittest
app:$(objects) main.o
	g++ -O2 -o app $(objects) main.o $(libs)
unittest:$(objects) test_main.o
	g++ -O2 -o unittest $(objects) test_main.o $(libs)


main.o:main.cpp *.h
test_main.o:test_main.cpp *.h
util.o:util.cpp *.h
urls.o:urls.cpp *.h

clean:
	rm -f *.o app unittest

