CC := g++

HDRS := -I./ \

CFLAGS :=

LDFLAGS := -fPIC -shared

LDDRS :=

LDLIBS := -lPocoFoundation -pthread

all: test

ProcessHandleThread.o : ProcessHandleThread.cc ProcessHandleThread.h
	$(CC) -c $(HDRS) $(CFLAGS) $< -o $@

test.o : test.cc
	$(CC) -c $(HDRS) $(CFLAGS) $< -o $@

test: ProcessHandleThread.o test.o
	$(CC) $(LDDRS) $(LDLIBS) $^ -o $@


clean:
	-rm -f test ProcessHandleThread.o test.o
