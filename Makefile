CC = gcc
CFLAGS = -std=c11 -Iinclude
LDFLAGS = -lm
SRC = src/main.c src/ran.c src/state.c
OUT = crng
PREFIX ?= /usr/local/
DESTDIR ?= 


all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

install: $(OUT)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 0755 $(OUT) $(DESTDIR)$(PREFIX)/bin/$(OUT)
	@echo "Installed $(OUT) to $(DESTDIR)$(PREFIX)/bin"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(OUT)
	@echo "Uninstalled $(OUT) from $(DESTDIR)$(PREFIX)/bin"

clean:
	rm -f $(OUT)

.PHONY: all install uninstall clean
