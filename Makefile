CC=gcc
CFLAGS=-Wall -O2 -I.

build: main.c server.c client.c
	$(CC) main.c server.c client.c -o dgramwifi

