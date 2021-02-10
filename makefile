CC = gcc
OBJS = main.o Solver.o Generate.o Parser.o Prints.o Logic.o LogicAux.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Generate.h Prints.h Parser.h Solver.h Logic.h LogicAux.h 
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c Solver.h  
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Generate.o: Generate.c Generate.h 
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h 
	$(CC) $(COMP_FLAG) -c $*.c
Prints.o: Prints.c Prints.h 
	$(CC) $(COMP_FLAG) -c $*.c
Logic.o: Logic.c Logic.h 
	$(CC) $(COMP_FLAG) -c $*.c
LogicAux.o: LogicAux.c LogicAux.h
	$(CC) $(COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)

	
