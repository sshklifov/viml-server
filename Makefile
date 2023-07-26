.PHONY: all
all: viml-server

viml-server: tok.c tree.c Node.h
	g++ tree.c -ggdb -o "viml-server"

tok.c: tok.l
	flex -L -o tok.c tok.l

tree.c: tree.y
	bison -l -o tree.c tree.y

.PHONY: clean
clean:
	rm -f tree.c tok.c viml-server

.PHONY: test
test:
	./viml-server < test.txt
