CC := gcc
CFLAGS := -g -Wall \
	-std=c99 \
	-Wextra -Werror \
	-Wconversion \
	-Wno-unused-parameter \
	-Wpointer-arith \
	-Wshadow \
	-Wwrite-strings \
	-march=native


OBJECTS := counter.o gauge.o
SRC_DIR := src

.PHONY: clean

counter.o: $(SRC_DIR)/counter.c $(SRC_DIR)/counter.h $(SRC_DIR)/spinlock.h $(SRC_DIR)/counter_private.h
	$(CC) $(CFLAGS) -c $^

gauge.o: $(SRC_DIR)/gauge.c $(SRC_DIR)/gauge.h $(SRC_DIR)/counter_private.h
	$(CC) $(CFLAGS) -c $^

counter: counter.o $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $^

gauge: gauge.o counter.o $(SRC_DIR)/gauge_main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm $(OBJECTS) counter
