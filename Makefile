CROSS ?= arm-obreey-linux-gnueabi
CC = $(CROSS)-gcc
CFLAGS = -Wall -DDEBUG=1

test: test.c libinkview-compat.so
	$(CC) $(CFLAGS) -linkview -linkview-compat -L. $< -o $@

libinkview-compat.so: inkview-compat.c
	$(CC) $(CFLAGS) -shared -lpthread -linkview -s -fPIC $< -o $@

clean:
	rm -f test *.so
