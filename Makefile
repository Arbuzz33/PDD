CC = gcc
CFLAGS = -Wall -g
TARGET = pdd
SRC = main.c help.c disk_info.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
