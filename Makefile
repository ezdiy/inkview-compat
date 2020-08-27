CROSS ?= arm-obreey-linux-gnueabi
CC = $(CROSS)-gcc
CFLAGS = -Wall -O2

libinkview-compat.so: inkview-compat.c
	$(CC) $(CFLAGS) -shared -lpthread -linkview -fPIC -s $< -o $@

clean:
	rm -f *.so
