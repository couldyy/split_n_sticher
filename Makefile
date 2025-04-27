CC=gcc

all: splitNsticher

splitNsticher: splitNsticher.c utils.c utils.h
	$(CC) -o splitNsticher splitNsticher.c utils.c utils.h

