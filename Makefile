CXX=gcc
all: v 
v:
	$(CXX) memlock.c va/vmem_access.c va/vmem_parser.c -D_GNU_SOURCE -O3 -o ml -Wall -Wextra -Wpedantic -Werror -Wno-unused-result -std=c99 

clean:
	rm -f ml
