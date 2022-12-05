CC = gcc
CFLAGS = -g -Wall -lpthread -lutil

.PHONY: clean all


visualizer: visualizer.o
	$(CC) $^ -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -rf