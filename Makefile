OBJS = demo.o 
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

demo: demo.cpp
	g++ $(CFLAGS) $(LIBS) -o $@ $<

clean:
	rm -rf demo demo.o
