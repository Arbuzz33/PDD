CC = gcc
CFLAGS = -Wall -g
TARGET = pdd
SRC = main.c explorer.c help.c disk_info.c format.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)
	sudo chmod +x test_speed.sh

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

install:
	sudo cp $(TARGET) /usr/local/bin
	sudo cp test_speed.sh /usr/local/bin
	sudo chmod +x /usr/local/bin/$(TARGET)
	sudo chmod +x /usr/local/bin/test_speed.sh

delete:
	sudo rm -f /usr/local/bin/$(TARGET)
	sudo rm -f /usr/local/bin/test_speed.sh
