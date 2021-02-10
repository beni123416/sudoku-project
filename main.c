
#include "Prints.h"
#include "Generate.h"
#include "Logic.h"
#include "LogicAux.h"
#include "Parser.h"
#include "Solver.h"
#include "time.h"



int main()
{
	Game *game;
	srand(time(0));
	game = init_Game(NULL);
	printWelcome();
	readCommand(game);
	return 0;
}
