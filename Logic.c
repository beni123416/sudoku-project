#include "Logic.h"


void solve(Game *game, char* path)
{
	bool flag = true;
	puzzle* game_board;
	int* MN = readMN(path);
	if (!MN)
	{
		return;
	}

	
	game_board = init_puzzle(MN[0], MN[1]);
	free(MN);
	flag = fileToPuzzle(path, game_board);
	if (!flag)
	{
		free_game_board(game_board);
		return;
	}
	if (!validBoard(game_board,true))
	{
		printInvalidBoard();
		free_game_board(game_board);
		return;
	}
	freeInsideGame(game);
	game->game_board = game_board;
	game->mode = 2;
	initInsideGame(game);
	print_board(game);
	ifBoardIsCompleted(game);
}

void edit(Game* game, char* path)
{
	bool flag = true;
	int* MN;
	puzzle* game_board;
	if (path == NULL)
	{
		game_board = init_puzzle(3, 3);
		freeInsideGame(game);
		game->game_board = game_board;
		initInsideGame(game);
	}
	else
	{
		MN = readMN(path);
		if (!MN)
		{
			return;
		}
		game_board = init_puzzle(MN[0], MN[1]);
		free(MN);
		flag=fileToPuzzle(path,game_board);
		if (!flag)
		{
			free_game_board(game_board);
			return;
		}
		freeInsideGame(game);
		game->game_board = game_board;
		initInsideGame(game);

		/*if (!validBoard(game->game_board))
		{
			printInvalidBoard();
			return;
		}*/
	}
	game->mode = 1;
	print_board(game);
}

void mark_errors(puzzle* game_board, int x)
{
	game_board->markErrors = x;
}

void print_board(Game* game)
{
	int i, j, k, sep = game->game_board->cols;
	for (k = 0; k < 4 * game->game_board->size + sep; k++)
	{
		printf("-");
	}
	printf("-\n");
	for (i = 0; i < game->game_board->size; i++)
	{
		for (j = 0; j < game->game_board->size; j++)
		{
			if (j == 0)
			{
				printf("|");
			}
			if (game->game_board->mat[i][j].val == -1)
			{
				printf("    ");
			}
			else
			{
				if (game->game_board->mat[i][j].fixed)
				{
					if (game->mode == 1)
					{
						if (game->game_board->mat[i][j].marked == true)
						{
							printf(" %2d*", game->game_board->mat[i][j].val);
						}
						else
						{
							printf(" %2d ", game->game_board->mat[i][j].val);
						}

					}
					else
					{
						printf(" %2d.", game->game_board->mat[i][j].val);

					}
				}
				else
				{
					if (game->game_board->mat[i][j].marked == true && (game->game_board->markErrors == 1 || game->mode == 1))
					{
						printf(" %2d*", game->game_board->mat[i][j].val);
					}
					else
					{
						printf(" %2d ", game->game_board->mat[i][j].val);
					}

				}
			}
			if ((j + 1) % game->game_board->cols==0)
			{
				if (j == game->game_board->size - 1)
				{
					printf("|\n");
				}
				else
				{
					printf("|");
				}
			}

		}
		if ((i + 1) % game->game_board->rows==0)
		{
			for (k = 0; k < 4 * game->game_board->size + sep; k++)
			{
				printf("-");
			}
			printf("-\n");
		}

	}

}

void print_board_testing(puzzle* game_board)
{
	int i, j, k, sep = game_board->cols;
	for (k = 0; k < 4 * game_board->size + sep; k++)
	{
		printf("-");
	}
	printf("-\n");
	for (i = 0; i < game_board->size; i++)
	{
		for (j = 0; j < game_board->size; j++)
		{
			if (j == 0)
			{
				printf("|");
			}
			if(game_board->solMat[i][j].val==-1)
			{
				printf("    ");
			}
			else
			{
				if (game_board->solMat[i][j].fixed)
				{
					printf(" .%2d", game_board->mat[i][j].val);
				}
				else
				{
					if (game_board->solMat[i][j].marked == true && (game_board->markErrors == 1))
					{
						printf(" *%2d", game_board->solMat[i][j].val);
					}
					else
					{
						printf("  %2d", game_board->solMat[i][j].val);
					}

				}
			}
			if ((j + 1) % game_board->rows == 0)
			{
				if (j == game_board->size - 1)
				{
					printf("|\n");
				}
				else
				{
					printf("|");
				}
			}

		}
		if ((i + 1) % game_board->cols == 0)
		{
			for (k = 0; k < 4 * game_board->size + sep; k++)
			{
				printf("-");
			}
			printf("-\n");
		}

	}

}

void set(int x, int y, int z, Game *game, bool update)
{
	int old_val;
	if (x < 0 || x >=  game->game_board->size) /*stage3*/
	{
		ParamError(x, game->game_board->size);
		return;
	}
	else
	{
		if (y < 0 || y >= game->game_board->size) /*stage3*/
		{
			ParamError(y, game->game_board->size);
			return;
		}
		else
		{
			if (z < 0 || y >= game->game_board->size) /*stage3*/
			{
				setParamZError(z, game->game_board->size);
				return;
			}
		}
	}
	if (game->game_board->mat[x][y].fixed && game->mode==2)/*stage4*/
	{
		printFixedError();
		return;
	}
	else
	{
		old_val = game->game_board->mat[x][y].val;
		if (update)
		{
			updateList(game,"set",x,y, game->game_board->mat[x][y].val, z, 0);
		}
		if (game->game_board->mat[x][y].val != -1)
		{
			clearCell(x, y, old_val, game);
		}
		if(z!=0)
		{
			game->game_board->mat[x][y].val = z;
			updateMarked(game->game_board, x, y, z, old_val);
		}
		if (update)
		{
			print_board(game);
		}
	}
}

void validate(Game* game)
{
	int flag = 0;
	if (!validBoard(game->game_board,false))
	{
		printInvalidBoard();
		return;
	}
	flag = ILP(game->game_board);
	if (flag != 0)
	{
		printUnsolvableBoard();
		return;
	}
	if (flag == 0)
	{
		printSolvableBoard();
	}

}

void guess(Game* game, double x)
{
	int flag = 0;
	int i, j, num;
	double*** prob;
	int count = 0;
	if (x<0.0 || x>1.0)
	{
		printInvalidThreshold();
		return;
	}
	if (!validBoard(game->game_board,false))
	{
		printInvalidBoard();
		return;
	}
	prob = (double***)malloc(game->game_board->size * sizeof(double**));
	if (!prob)
	{
		printFuncError("malloc");
		return;
	}
	for (i = 0; i < game->game_board->size; i++)
	{
		prob[i] = (double**)malloc(game->game_board->size * sizeof(double*));
		if (!prob[i])
		{
			printFuncError("malloc");
			freeProb(prob, game->game_board->size);
			return;
		}
	}
	for (i = 0; i < game->game_board->size; i++)
	{
		for (j = 0; j < game->game_board->size; j++)
		{
			prob[i][j] = (double*)malloc(game->game_board->size * sizeof(double));
			if (!prob[i][j])
			{
				printFuncError("malloc");
				freeProb(prob, game->game_board->size);
				return;
			}
		}
	}
	flag = LP(game->game_board, prob);
	if (flag != 0)
	{
		printUnsolvableBoard();
		freeProb(prob, game->game_board->size);
		return;
	}
	for (i = 0; i < game->game_board->size; i++)
	{
		for (j = 0; j < game->game_board->size; j++)
		{
			num = chooseVal(game->game_board, prob, i, j, x);
			/*need to announce and stop?*/
			if (num > -1)
			{
				game->game_board->mat[i][j].val = num;
				updateList(game, "guess", i, j,-1,num, count);
				count++;
			}
		}
	}
	freeProb(prob, game->game_board->size);
	print_board(game);
}

void generate(Game* game, int x, int y)
{
	int flag = 0;
	int* index;
	int range, i, count = 0, gen = 0;
	int value = -1;
	range = game->game_board->size * game->game_board->size;
	if (x > range || x<0 || y>range || y < 0)
	{
		printRangeError(range);
		return;
	}
	if (range - FullCells(game->game_board) < x)
	{
		printNotEnoughEmptyCells();
		return;
	}
	if (!validBoard(game->game_board,false))
	{
		printInvalidBoard();
		return;
	}
	while (gen < 1000)
	{
		count = 0;
		for (i = 0; i < x; i++)
		{

			index = randEmptyCell(game->game_board);
			while (!index)
			{
				index = randEmptyCell(game->game_board);
			}
			
			value = randValue(game->game_board, index[0], index[1]);

			if (value == -1)
			{
				free(index);
				index = NULL;
				
				gen++;
				if (i > 0)
				{
					undo(game);
				}
				break;
			}
			else
			{

				game->game_board->mat[index[0]][index[1]].val = value;

				updateList(game, "generate", index[0], index[1],-1,value,count);

				count++;
				free(index);
				index = NULL;
			}
			
		}
		if (value == -1)
		{
			continue;
		}

		flag = ILP(game->game_board);

		if (flag != 0)
		{
			gen++;

			undo(game);

			continue;
		}

		copyMat(game->game_board->mat, game->game_board->solMat, game->game_board->size,true,game,count);

		count = count + ((game->game_board->size) * (game->game_board->size));
		for (i = 0; i < range - y; i++)
		{

			index = randNotEmptyCell(game->game_board);
			while (!index)
			{
				index = randNotEmptyCell(game->game_board);
			}
			updateList(game, "generate", index[0], index[1], game->game_board->mat[index[0]][index[1]].val,-1,count);

			game->game_board->mat[index[0]][index[1]].val = -1;
			free(index);
			index = NULL;
		}

		break;
	}

	if (gen == 1000)
	{
		printGenerationFailed();
	}
	else
	{
		print_board(game);
	}
}

void undo(Game* game)
{
	if (game->current_move->data->index==-1)
	{
		printNoMoreUndo();
		return;
	}
	while (game->current_move->data->index > 0)
	{
		undoCell(game->current_move->data->x, game->current_move->data->y, game->current_move->data->before, game);
		game->current_move = game->current_move->prev;
	}
	/* last undo when index=0 */
	/* when it's autofill that didnt change the board)*/
	if (game->current_move->data->x==-1 && game->current_move->data->y==-1)
	{
		game->current_move = game->current_move->prev;
	}
	else
	{
		undoCell(game->current_move->data->x, game->current_move->data->y, game->current_move->data->before, game);
		game->current_move = game->current_move->prev;
	}

}

void redo(Game* game)
{

	if (game->current_move == NULL || game->current_move->next == NULL)
	{
		printNoMoreRedo();
		return;
	}

	/* first redo when index=0 */
	game->current_move = game->current_move->next;
	/*if its not an autofill that didnt change the board*/
	if (game->current_move->data->x!=-1 && game->current_move->data->y!=-1)
	{
		redoCell(game->current_move->data->x, game->current_move->data->y, game->current_move->data->after, game);
	}
	while (game->current_move->next != NULL && game->current_move->next->data->index != 0)
	{
		game->current_move = game->current_move->next;
		redoCell(game->current_move->data->x, game->current_move->data->y, game->current_move->data->after, game);
	}
}

void save(Game* game, char* path)
{
	int flag = 0;
	if (game->mode == 1)
	{
		if (!validBoard(game->game_board,false))
		{
			printInvalidBoard();
			return;
		}
		flag = ILP(game->game_board);
		if (flag != 0)
		{
			printUnsolvableBoard();
			return;
		}
		puzzleTofile(game->game_board, path, game->mode);
	}
	else  /*game->mode ==  2*/
	{
		if (!validBoard(game->game_board, true))
		{
			printInvalidBoard();
			return;
		}
		puzzleTofile(game->game_board, path, game->mode);
	}

}

void hint(Game* game, int x, int y)
{
	int flag = 0;
	if (x > game->game_board->size - 1 || x<0 || y>game->game_board->size - 1 || y < 0)
	{
		printRangeError(game->game_board->size);
		return;
	}
	if (!validBoard(game->game_board,false))
	{
		printInvalidBoard();
		return;
	}
	if (game->game_board->mat[x][y].fixed)
	{
		printFixedError();
		return;
	}
	if (game->game_board->mat[x][y].val != -1)
	{
		printContainVal();
		return;
	}
	flag = ILP(game->game_board);
	if (flag != 0)
	{
		printUnsolvableBoard();
		return;
	}
	printValHint(game->game_board->solMat[x][y].val);
}

void guess_hint(Game* game, int x, int y)
{
	double*** prob;
	int flag = 0;
	int i,j;
	if (x > game->game_board->size-1 || x<0 || y>game->game_board->size -1 || y < 0)
	{
		printf("The row or column exceeded the range of the board\n");
		return;
	}
	if (!validBoard(game->game_board,false))
	{
		printInvalidBoard();
		return;
	}
	if (game->game_board->mat[x][y].fixed)
	{
		printFixedError();
		return;
	}
	if (game->game_board->mat[x][y].val != -1)
	{
		printContainVal();
	}
	prob = (double***)malloc(game->game_board->size * sizeof(double**));
	if (!prob)
	{
		printFuncError("malloc");
		return;
	}
	for (i = 0; i < game->game_board->size; i++)
	{
		prob[i] = (double**)malloc(game->game_board->size * sizeof(double*));
		if (!prob[i])
		{
			printFuncError("malloc");
			freeProb(prob, game->game_board->size);
			return;
		}
	}
	for (i = 0; i < game->game_board->size; i++)
	{
		for (j = 0; j < game->game_board->size; j++)
		{
			prob[i][j] = (double*)malloc(game->game_board->size * sizeof(double));
			if (!prob[i][j])
			{
				printFuncError("malloc");
				freeProb(prob, game->game_board->size);
				return;
			}
		}
	}
	flag = LP(game->game_board, prob);
	if (flag != 0)
	{
		if (flag == 1)
		{
			printUnsolvableBoard();
		}
		freeProb(prob, game->game_board->size);
		return;
	}
	printAllLegalValues(game->game_board->size,  x,  y, prob);
	freeProb(prob, game->game_board->size);
}

void num_solutions(Game* game)
{
	int count = 0;
	if (!validBoard(game->game_board,false))
	{
		printInvalidBoard();
		return;
	}
	count = backTracking_exh(game->game_board);
	printNumOfSolve(count);
}

void autofill(Game* game)
{
	int i, j, num, count = 0;
	int** arr;
	if (!validBoard(game->game_board,false))
	{
		printInvalidBoard();
		return;
	}
	arr = (int**)malloc(game->game_board->size * sizeof(int*));
	if (!arr)
	{
		printFuncError("malloc");
		return;
	}
	for (i = 0; i < game->game_board->size; i++)
	{
		arr[i] = (int*)malloc(game->game_board->size * sizeof(int));
		if (!arr[i])
		{
			printFuncError("malloc");
			for (j = 0; j < i; j++)
			{
				free(arr[j]);
			}
			free(arr);
			return;
		}
	}
	for (i = 0; i < game->game_board->size; i++)
	{
		for (j = 0; j < game->game_board->size; j++)
		{
			arr[i][j] = 0;
			if (game->game_board->mat[i][j].val != -1)
			{
				continue;
			}
			count = 0;
			for (num = 1; num <= game->game_board->size; num++)
			{
				if (legalNum(game->game_board, num, i, j,false))
				{
					if (count > 0)
					{
						arr[i][j] = 0;
						break;
					}
					arr[i][j] = num;
					count++;
				}
			}
		}
	}
	count = 0;
	for (i = 0; i < game->game_board->size; i++)
	{
		for (j = 0; j < game->game_board->size; j++)
		{
			if (arr[i][j] > 0 && game->game_board->mat[i][j].val == -1)
			{
				game->game_board->mat[i][j].val = arr[i][j];
				updateList(game, "autofill", i, j, -1, arr[i][j], count);
				count++;
			}
		}
	}
	markedAllBoard(game->game_board);
	if (count==0)
	{
		updateList(game, "autofill", -1, -1, -1,-1, 0);
	}
	if (arr)
	{
		for (i = 0; i < game->game_board->size; i++)
		{
			if (arr[i])
			{
				free(arr[i]);
			}
		}
		free(arr);
	}
	print_board(game);
}

/*reset*/
void reset(Game* game)
{
	game->current_move = game->head_list;
	copyMat(game->game_board->mat, game->original_mat, game->game_board->size, false, game, -1);
	print_board(game);
}

void exitGame(Game* game)
{
	freeGame(game);
	printExitGame();
	exit(0);
}

/* -------------------------------------------------------------------------------------*/

void undoCell(int x, int y, int before, Game* game)
{
	if (before == -1)
	{
		set(x, y, 0, game,false);
	}
	else
	{
		set(x, y, before, game,false);
	}
	
}
void redoCell(int x, int y, int after, Game* game)
{
	if (after == -1)
	{
		set(x, y, 0, game, false);
	}
	else
	{
		set(x, y, after, game, false);
	}
}

