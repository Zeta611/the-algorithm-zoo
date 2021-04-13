MAIN := $(basename $(shell find ./ -name *.w))

CC := gcc
CFLAGS := -O3 -Wall -Wextra -Wpedantic -std=c17

PDFTEX := pdftex

all: prog doc

prog: $(MAIN) $(MAIN).fmt.c

$(MAIN): $(MAIN).o

$(MAIN).o: $(MAIN).c

$(MAIN).fmt.c: $(MAIN).c
	sed 's/\/\*[:0-9]*\*\///g;s/^#line.*//g' $< > $@
	clang-format -i $@

doc: $(MAIN).pdf

$(MAIN).pdf: $(MAIN).tex
	$(PDFTEX) $< -o $@

.PHONY: all prog doc clean
clean:
	rm -f *.idx *.log *.scn *.tex *.pdf *.toc *.o *.c $(MAIN)
