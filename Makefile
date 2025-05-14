CC = gcc
CFLAGS = -g -Wall -Wextra -I. -Imongoose
SRC = src/main.c src/handlers.c mongoose/mongoose.c
TARGET = time_calculator


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
