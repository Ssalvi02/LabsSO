CC=gcc -Wall

P1=ex1
P2=ex2

PROGS=$(P1) $(P2)

all: $(PROGS)

ex1:
	gcc ex1.c -o ex1 -lpthread
	
ex2:
	gcc ex2.c matriz.c -o ex2 -lpthread -lm
	
clean:
	rm $(PROGS)

clean_output:
	rm *.txt