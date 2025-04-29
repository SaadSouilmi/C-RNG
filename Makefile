CC = gcc-14
CFLAGS = -std=c11 -Iinclude
LDFLAGS = -lm -fopenmp
SRC = src/main.c src/ran.c src/state.c src/sample.c src/args.c
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
	rm ~/.ran_c/*.bin


.PHONY: all install uninstall clean
