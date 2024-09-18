
TARGET = program

SRC = src/main.c src/draw.c src/poker.c

CC = gcc

CFLAGS = -Iinclude -Wall -Wextra -O2 -lncurses

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) 

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

