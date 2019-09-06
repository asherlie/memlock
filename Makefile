CC=gcc
CFLAGS= -lmemcarve -Wall -Wextra -Wpedantic -Werror -Wno-unused-result -std=c99 -lpthread -O2

all: ml 
ml: memlock.c
	$(CC) $(CFLAGS) memlock.c -o ml

clean:
	rm -f ml
