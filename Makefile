CC = gcc
CFLAGS = -Wall -g

.PHONY: all
all: sw

sw: sw.c
	@$(CC) $(CFLAGS) -o sw sw.c 

clean:
	@rm sw
