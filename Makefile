CC = gcc
CFLAGS = -g -Wall -pthread -lutil

SRCFILES = visualizer.c ping.c signal_encoding.c
HEADERS = visualizer.h ping.h signal_encoding.h

.PHONY: clean all

all: visualizer

visualizer: $(SRCFILES) $(HEADERS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf visualizer