CC = clang
CFLAGS = -Werror-implicit-function-declaration -fsanitize=address -Iinclude
CFILES = $(shell find src/ -name "*.c")

.PHONY: clean

bin/nexcc: $(CFILES)
	@ mkdir -p bin
	$(CC) $(CFLAGS) $^ -o bin/nexcc

clean:
	rm bin/*
