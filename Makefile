CC = gcc
CFLAGS = -g -Wall -lpthread -lutil

.PHONY: clean all

all: visualizer

visualizer: visualizer.o signal_encoding.o
	$(CC) $^ -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -rf visualizer