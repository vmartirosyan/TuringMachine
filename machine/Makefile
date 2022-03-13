ARGS = -g -D_DEBUG_

all: main.cpp turing_machine.o
		g++ main.cpp turing_machine.o $(ARGS) -o turing_machine

turing_machine.o: turing_machine.h turing_machine.cpp
		g++ -c turing_machine.cpp $(ARGS)


.PHONY: clean all
clean: turing_machine turing_machine.o
		rm turing_machine turing_machine.o
