CXX=gcc
all: v 
v:
	$(CXX) memlock.c -lmemcarve -O2 -o ml -Wall -Wextra -Wpedantic -Werror -Wno-unused-result -std=c99 

clean:
	rm -f ml
