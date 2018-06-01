CC = gcc
CFLAGS = -Wall -g

.PHONY: all
all: sw

debug:
	@$(CC) $(CFLAGS) -ggdb -o sw sw.c 

sw: sw.c
	@$(CC) $(CFLAGS) -o sw sw.c 

clean:
	@rm sw
