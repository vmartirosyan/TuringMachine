ARGS = -g -D_DEBUG_

all: main.cpp compiler.o
		g++ main.cpp compiler.o $(ARGS) -o compiler

compiler.o: compiler.h compiler.cpp
		g++ -c compiler.cpp $(ARGS)


.PHONY: clean all
clean: compiler compiler.o
		rm compiler compiler.o
