CC=gcc
FLAGS=-s -std=gnu99
SRC=renameall.c color_shell/src/color_shell.c
OBJ=$(SRC:.c=.o)
BIN=bin/renameall

all: mkdir $(BIN)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS)

%.o: %.c
	$(CC) $< -o $@ -c $(FLAGS)

install: all
	cp -f $(BIN) /usr/bin

clean:
	rm -rf $(BIN) $(OBJ) bin

_clean:
	rm -rf $(OBJ)

mkdir:
	mkdir -p bin

.PHONY: clean _clean all
