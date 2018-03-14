CC=gcc
FLAGS=-s -std=gnu99
SRC=renameall.c color_shell/src/color_shell.c
OBJ=$(SRC:.c=.o)
BIN=bin/renameall

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS)

%.o: %.c
	$(CC) $< -o $@ -c $(FLAGS)

clean:
	rm -rf $(BIN) $(OBJ)

_clean:
	rm -rf $(OBJ)

.PHONY: clean _clean all
