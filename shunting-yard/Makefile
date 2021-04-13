MAIN := $(basename $(shell find ./ -name *.w))

CC := gcc
CFLAGS := -O3 -Wall -Wextra -Wpedantic -std=c17

SED := /bin/sed
CAT := /bin/cat
CP := /bin/cp
CLANGFORMAT := /bin/clang-format

PDFTEX := pdftex

all: prog doc fmt

fmt: $(MAIN).c

prog: $(MAIN)

$(MAIN): $(MAIN).o

$(MAIN).o: $(MAIN).orig.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(MAIN).c: $(MAIN).orig.c
	# Remove section-indicating comments & line directives, suppress
	# duplicate empty lines, and then remove the first and/or the last line
	# if they are empty.
	$(SED) 's/\(\/\*[:0-9]*\*\/\|^#line.*\)//g' $< \
		| $(CAT) -s \
		| $(SED) '1{/^$$/d};$${/^$$/d}'\
		> $@
	$(CLANGFORMAT) -i $@

$(MAIN).orig.c: $(MAIN).w
	$(CTANGLE) $<
	$(CP) $(<:w=c) $@

doc: $(MAIN).pdf

$(MAIN).pdf: $(MAIN).tex
	$(PDFTEX) $< -o $@

.PHONY: all prog doc clean
clean:
	rm -f *.idx *.log *.scn *.tex *.pdf *.toc *.o *.c $(MAIN)
