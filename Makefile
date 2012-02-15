CC=gcc
# using libcurl
CFLAGS=-lcurl 

all: pastebin

pastebin:
	$(CC) $(CFLAGS) main.c -o pastebin

clean:
	rm -rf  pastebin
