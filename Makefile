CC = gcc
CFLAGS = -g -Wall -Wextra -I. -Imongoose
SRC = src/main.c src/handlers.c mongoose/mongoose.c
TARGET = time_calculator
PORT = 8000

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	@-rm -f $(TARGET)
	@-fuser -k -n tcp $(PORT) 2>/dev/null || true

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run