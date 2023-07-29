# counterexamples=-Wcounterexamples"
counterexamples=

.PHONY: default
default: viml-server

lexer_viml.c: lex.l
	flex -L -o lexer_viml.c lex.l

lexer_viml.o: lexer_viml.c
	g++ -c -ggdb lexer_viml.c -o lexer_viml.o

parser_viml.c: parse.y
	bison ${counterexamples} -l -o parser_viml.c parse.y

viml-server: lexer_viml.o parser_viml.c Node.h
	g++ -ggdb parser_viml.c lexer_viml.o -o "viml-server"

lexer_help.c: help.l
	flex -L -o lexer_help.c help.l

lexer_help.o: lexer_help.c
	g++ -c -ggdb lexer_help.c -o lexer_help.o

parser_help.c: help.y
	bison ${counterexamples} -l -o parser_help.c help.y

help: lexer_help.o parser_help.c
	g++ -ggdb lexer_help.o parser_help.c -o help

.PHONY: clean
clean:
	rm -f lexer_*.c parser_*.c *.o viml-server help

.PHONY: test
test:
	./viml-server < test.txt

.PHONY: all
all: viml-server help
