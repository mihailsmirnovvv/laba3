CC = gcc
CFLAGS = -g -Wall -Wextra
LIBS = -lssl -lcrypto

all: time_calculator

time_calculator: main.c mongoose.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f time_calculator

run: time_calculator
	./time_calculator

.PHONY: all clean run
