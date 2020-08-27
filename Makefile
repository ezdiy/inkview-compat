CROSS ?= arm-obreey-linux-gnueabi
CC = $(CROSS)-gcc
CFLAGS = -Wall -O2

libinkview-compat.so: inkview-compat.c
	$(CC) $(CFLAGS) -shared -linkview -fPIC -s $< -o $@

clean:
	rm -f *.so
