CROSS ?= arm-obreey-linux-gnueabi
CC = $(CROSS)-gcc
CFLAGS = -Wall

test: test.c libinkview-compat.so
	$(CC) -linkview -linkview-compat -L. $< -o $@

libinkview-compat.so: inkview-compat.c
	$(CC) -shared -linkview -s -fPIC $< -o $@

clean:
	rm -f test *.so
