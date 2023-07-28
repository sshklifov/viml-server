.PHONY: all
all: viml-server

viml-server: lex.c tree.c Node.h
	g++ tree.c -ggdb -o "viml-server"

lex.c: lex.l
	flex -L -o lex.c lex.l

tree.c: tree.y
	bison -l -o tree.c tree.y

.PHONY: clean
clean:
	rm -f tree.c lex.c viml-server

.PHONY: test
test:
	./viml-server < test.txt


help.c: help.l
	flex -L -o help.c help.l

help: help.c
	g++ help.c -o help
