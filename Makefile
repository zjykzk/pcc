CC := gcc
CFLAGS := -g -Wall -Wextra -Werror -Wconversion -Wno-unused-parameter -Wpointer-arith -Wshadow -Wwrite-strings -march=native


OBJECTS := counter.o
SRC_DIR := src

.PHONY: clean

counter.o: $(SRC_DIR)/counter.c $(SRC_DIR)/counter.h
	$(CC) $(CFLAGS) -c $^

counter: counter.o $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm $(OBJECTS) counter
