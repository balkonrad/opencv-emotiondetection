OBJS = demo.o 
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: demo fisherface

demo: demo.cpp
	g++ $(CFLAGS) $(LIBS) -o $@ $<

fisherface: fisherface.cpp
	g++ $(CFLAGS) $(LIBS) -o $@ $<

clean:
	rm -rf demo demo.o
