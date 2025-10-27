CC = gcc

CFLAGS = -Wall -Wextra -std=c11


SRC_DIR = src
BIN_DIR = bin
DATA_DIR = data


TARGET = $(BIN_DIR)/banco


SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/banco.c \
       $(SRC_DIR)/io.c


OBJS = $(SRCS:.c=.o)


all: setup $(TARGET)


setup:
	mkdir -p $(BIN_DIR)
	mkdir -p $(DATA_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(TARGET)

run: all
	./$(TARGET)


