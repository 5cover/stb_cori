CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -Og

EXAMPLE = example

# Default rule
debug: $(EXAMPLE)

release: CFLAGS = -Wall -Wextra -pedantic -O3
release: $(EXAMPLE)

$(EXAMPLE): $(EXAMPLE).c stb_cori.h
	$(CC) -o $(EXAMPLE) $(EXAMPLE).c $(CFLAGS)

clean:
	rm $(EXAMPLE)