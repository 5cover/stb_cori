CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fanalyzer

TEST=tests/main
EXAMPLE = example

# Default rule
debug: CFLAGS := $(CFLAGS) -g -Og
debug: $(EXAMPLE)

release: CFLAGS := $(CFLAGS) -O3
release: $(EXAMPLE)

preprocess: $(EXAMPLE).c stb_cori.h
	$(CC) -o $(EXAMPLE)_preprocessed.i $(EXAMPLE).c $(CFLAGS) -E

$(EXAMPLE): $(EXAMPLE).c stb_cori.h
	$(CC) -o $(EXAMPLE) $(EXAMPLE).c $(CFLAGS)

test: $(wildcard tests/*.c)
	$(CC) -o $(TEST) $^ $(CFLAGS) 

clean:
	rm -f $(EXAMPLE) $(EXAMPLE)_preprocessed.i $(TEST)