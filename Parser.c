#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"
#include "Logic.h"


void readCommand(Game *game)
{
	char com[258] = { 0 };
	/*char *params[255];*/
	char *command, *param1, *param2, *param3, *param4;
	com[257] = '\0';
	while (true)
	{
		printPrompt();
		if (fgets(com, 258, stdin) == NULL)
		{
			break;
		}
		if (com == NULL)
		{
			printFuncError("fgets");
			continue;
		}
		if (com[257] != '\0')
		{
			printTooManyCharacters();
			com[257] = '\0';
			continue;
		}
		if (com[0] == '\n' || (com[0] == '\r' && com[1] == '\n'))
		{
			continue;
		}
		command = strtok(com, " \t\r\n");
		if (command == NULL)
		{
			continue;
		}
		/*for (i = 0; i < 255; i++)
		{
			params[i]= strtok(NULL, " \t\r\n");
		}
		char * extra = pstrtok(NULL, " \t\r\n");
		if (extra != NULL)
		{
			printExtraChar();
		}*/
		param1 = strtok(NULL, " \t\r\n");
		param2 = strtok(NULL, " \t\r\n");
		param3 = strtok(NULL, " \t\r\n");
		param4 = strtok(NULL, " \t\r\n");
		switch (hash_command(command))
		{
		case e_solve:
			if (!param1 || param2)
			{
				printWrongParams(1);
				continue;
			}
			solve(game, param1);
			break;

		case e_edit:
			if (param2)
			{
				printWrongParams(-1);
				continue;
			}
			edit(game, param1);
			break;

		case e_mark_errors:
			if (!check_mode(0, 0, 1, game->mode))
			{
				continue;
			}
			if (!param1 || param2)
			{
				printWrongParams(1);
				continue;
			}
			if (!isInt(param1))
			{
				printNotIntParam();
				continue;
			}
			if (atoi(param1) != 0 && atoi(param1) != 1)
			{
				printParam01();
			}
			mark_errors(game->game_board, atoi(param1));
			print_board(game);
			break;

		case e_print_board:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			print_board(game);
			break;

		case e_set:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (!param1 || !param2 || !param3 || param4)
			{
				printWrongParams(3);
				continue;
			}

			if (isInt(param1) && (atoi(param1) > game->game_board->size || atoi(param1) < 1))
			{
				printValueBetween("column", 1, game->game_board->size);
				continue;
			}
			if (isInt(param2) && (atoi(param2) > game->game_board->size || atoi(param2) < 1))
			{
				printValueBetween("row", 1, game->game_board->size);
				continue;
			}
			if (isInt(param3) && (atoi(param3) > game->game_board->size || atoi(param3) < 0))
			{
				printValueBetween("cell", 0, game->game_board->size);
				continue;
			}
			if (!isInt(param1) || !isInt(param2) || !isInt(param3))
			{
				printNotIntParam();
				continue;
				
			}
			set(atoi(param2) - 1, atoi(param1) - 1, atoi(param3), game, true);
			ifBoardIsCompleted(game);
			break;

		case e_validate:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			validate(game);
			break;

		case e_guess:
			if (!check_mode(0, 0, 1, game->mode))
			{
				continue;
			}
			if (!param1 || param2)
			{
				printWrongParams(1);
				continue;
			}
			if (!isFloat(param1))
			{
				printNotFloatParam();
				continue;
			}
			guess(game, atof(param1));
			ifBoardIsCompleted(game);
			break;

		case e_generate:
			if (!check_mode(0, 1, 0, game->mode))
			{
				continue;
			}
			if (!param1 || !param2 || param3)
			{
				printWrongParams(2);
				continue;
			}
			if (!isInt(param1) || !isInt(param2))
			{
				printNotIntParam();
				continue;
			}
			generate(game, atoi(param1), atoi(param2));
			break;

		case e_undo:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			undo(game);
			print_board(game);
			break;

		case e_redo:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			redo(game);
			print_board(game);
			break;

		case e_save:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (!param1 || param2)
			{
				printWrongParams(1);
				continue;
			}
			save(game, param1);
			break;

		case e_hint:
			if (!check_mode(0, 0, 1, game->mode))
			{
				continue;
			}
			if (!param1 || !param2 || param3)
			{
				printWrongParams(2);
				continue;
			}
			if (isInt(param1) && (atoi(param1) > game->game_board->size || atoi(param1) < 1))
			{
				printValueBetween("column", 1, game->game_board->size);
				continue;
			}
			if (isInt(param2) && (atoi(param2) > game->game_board->size || atoi(param2) < 1))
			{
				printValueBetween("row", 1, game->game_board->size);
				continue;
			}
			if (!isInt(param1) || !isInt(param2))
			{
				printNotIntParam();
				continue;
			}
			hint(game, atoi(param1)-1, atoi(param2)-1);
			break;

		case e_guess_hint:
			if (!check_mode(0, 0, 1, game->mode))
			{
				continue;
			}
			if (!param1 || !param2 || param3)
			{
				printWrongParams(2);
				continue;
			}
			if (isInt(param1) && (atoi(param1) > game->game_board->size || atoi(param1) < 1))
			{
				printValueBetween("column", 1, game->game_board->size);
				continue;
			}
			if (isInt(param2) && (atoi(param2) > game->game_board->size || atoi(param2) < 1))
			{
				printValueBetween("row", 1, game->game_board->size);
				continue;
			}
			if (!isInt(param1) || !isInt(param2))
			{
				printNotIntParam();
				continue;
			}
			guess_hint(game, atoi(param1)-1, atoi(param2)-1);
			break;

		case e_num_solutions:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			num_solutions(game);
			break;

		case e_autofill:
			if (!check_mode(0, 0, 1, game->mode))
			{
				continue;
			}
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			autofill(game);
			ifBoardIsCompleted(game);
			break;

		case e_reset:
			if (!check_mode(0, 1, 1, game->mode))
			{
				continue;
			}
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			reset(game);
			break;

		case e_exit_game:
			if (param1)
			{
				printWrongParams(0);
				continue;
			}
			exitGame(game);
			break;

		case e_none:
			printInvalidCommand();
			continue;
			break;
		}
	}
	exitGame(game);
}

int* readMN(char* path)
{
	int count = 0;
	char* ptr = NULL;
	char buff[4000];
	FILE* fptr;
	int* MN =(int*)malloc(sizeof(int)*2);
	if (!MN)
	{
		printFuncError("malloc");
		return NULL;
	}
	fptr = fopen(path, "r+");
	if (fptr == NULL)
	{
		printFileError();
		free(MN);
		return NULL;
	}

	while (count < 2)
	{
		ptr = strtokForFile(fptr, ptr, " \t\r\n", buff);
		if (ptr == NULL)
		{
			printNotEnoughInLine();
			free(MN);
			return NULL;
		}
		if (ptr[0] != '0' && atoi(ptr) == 0) /*n isn't digit*/
		{
			printNotDigitError();
			free(MN);
			return NULL;
		}
		if (atoi(ptr) < 1 || atoi(ptr) > 5) /* 0 < n,m < 6*/
		{
			printTooBig();
			free(MN);
			return NULL;
		}
		MN[count] = atoi(ptr);
		count++;
	}
	fclose(fptr);
	return MN;
}

bool IfFix1(puzzle* game_board, char* x, char* res0,int col,int row)
{
	if (strlen(x) > 2)
	{
		printTooManyDigits();
		return false;
	}
	game_board->mat[row][col].fixed = true;
	if (x[0] != '0' && atoi(res0) == 0)
	{
		printIllegalFormat();
		return false;
	}
	if (atoi(res0) == 0)
	{
		printIllegalFormat();
		return false;
	}
	else
	{
		if (atoi(res0) < 1 || atoi(res0) > game_board->size)
		{
			printRangeError(game_board->size);
			return false;
		}
		game_board->mat[row][col].val = atoi(res0);
	}
	return true;
}

bool IfFix2(puzzle* game_board, char* x, char* res0, char* res1, int col, int row)
{
	if (strlen(x) > 3)
	{
		printTooManyDigits();
		return false;
	}
	game_board->mat[row][col].fixed = true;
	if ((x[0] != '0' && atoi(res0) == 0) || (x[1] != '0' && atoi(res1) == 0)) /*if input is char*/
	{
		printIllegalFormat();
		return false;
	}
	if (atoi(res0) == 0)
	{
		printSingleDigitError();
		return false;
	}
	else
	{
		res0[0] = x[0];
		res0[1] = x[1];
		if (atoi(res0) < 1 || atoi(res0) > game_board->size)
		{
			printRangeError(game_board->size);
			return false;
		}
		game_board->mat[row][col].val = atoi(res0);
	}
	return true;
}

bool IfNotFix1(puzzle* game_board, char* x, char* res0, int col, int row)
{
	if (strcmp(res0, "0") != 0 && atoi(x) == 0)
	{
		printIllegalFormat();
		return false;
	}

	if (atoi(res0) == 0)
	{
		game_board->mat[row][col].val = -1;
	}

	else
	{
		if (atoi(x) < 1 || atoi(x) > game_board->size)
		{
			printRangeError(game_board->size);
			return false;
		}
		game_board->mat[row][col].val = atoi(x);
	}
	return true;
}

bool IfNotFix2(puzzle* game_board, char* x, char* res0, char* res1, int col, int row)
{
	if ((x[0] != '0' && atoi(res0) == 0) || (x[1] != '0' && atoi(res1) == 0)) /*if input is char*/
	{
		printTooManyDigits();
		return false;
	}
	if (atoi(res0) == 0)
	{
		printSingleDigitError();
		return false;
	}
	else
	{
		if (atoi(x) < 1 || atoi(x) > game_board->size)
		{
			printRangeError(game_board->size);
			return false;
		}
		game_board->mat[row][col].val = atoi(x);
	}
	return true;
}

bool fileToCells(puzzle* game_board, char* x, char* res0, char* res1,  char* res2, int col, int row)
{

	if (strlen(x) > 1 && strcmp(res1, ".") == 0) /*fixed && single digit*/
	{
		if (!IfFix1(game_board, x, res0, col, row))
		{
			return false;
		}
	}
	else
	{
		if (strlen(x) > 2 && strcmp(res2, ".") == 0) /*fixed && 2 digit*/
		{
			if (!IfFix2(game_board, x, res0, res1, col, row))
			{
				return false;
			}
		}
		else/*not fixed*/
		{
			res0[1] = '\0';
			if (strlen(x) == 1) /*1 digit*/
			{
				if (!IfNotFix1(game_board, x, res0, col, row))
				{
					return false;
				}
			}
			else
			{
				if (strlen(x) == 2) /*2 digit*/
				{
					if (!IfNotFix2(game_board, x, res0, res1, col, row))
					{
						return false;
					}
				}
				else /*illegal*/
				{
					printTooManyDigits();
					return false;
				}
			}
		}
	}
	return true;
}

bool fileToPuzzle(char* path, puzzle* game_board)
{
	int row = 0, col = 0, count_mn = 0;
	char res0[4] = " ", res1[4] = " ", res2[4] = " ";
	char* ptr = NULL;
	char buff[4000];
	FILE* fptr;
	fptr = fopen(path, "r+");
	if (fptr == NULL)
	{
		printFileError();
		return false;
	}
	ptr = strtokForFile(fptr, ptr, " \t\r\n", buff);
	while (ptr != NULL)
	{
		if (count_mn < 2)
		{
			count_mn++;
			ptr = strtokForFile(fptr, ptr, " \t\r\n", buff);
			continue;
		}
		if (row == game_board->size)
		{
			printLongError();
			return false;
		}
		if (strlen(ptr) > 3)
		{
			printTooManyDigits();
			return false;
		}
		res0[0] = ptr[0];
		res1[0] = ptr[1];
		res2[0] = ptr[2];
		if (!fileToCells(game_board, ptr, res0, res1, res2, col, row))
		{
			return false;
		}
		col = (col + 1) % game_board->size;
		if (col == 0)
		{
			row++;
		}
		ptr = strtokForFile(fptr, ptr, " \t\r\n", buff);
	}
	if (col != 0 || row != game_board->size)
	{
		printShortError();
		return false;
	}

	fclose(fptr);
	markedAllBoard(game_board);
	return true;
}

char* strtokForFile(FILE* fptr, char* ptr, const char* delimiters, char buff[])
{
	if (ptr)
	{
		ptr = strtok(NULL, delimiters);
		if (ptr)
		{
			return ptr;
		}
	}
	do
	{
		memset(buff, 0, 4000);
		fgets(buff, 4000, fptr);
		ptr = strtok(buff, delimiters);
		if (ptr)
		{
			return ptr;
		}
	} while (!feof(fptr));
	return NULL;
}

void puzzleTofile(puzzle* game_board, char* path,int mode)
{
	int i, j;
	FILE* fptr;
	fptr = fopen(path, "w");
	if (fptr == NULL)
	{
		printFileError();
		return;
	}
	fprintf(fptr, "%d %d\n", game_board->rows, game_board->cols);
	for (i = 0; i < game_board->size; i++)
	{
		for (j = 0; j < game_board->size; j++)
		{
			if (game_board->mat[i][j].fixed || (mode==1 && game_board->mat[i][j].val!=-1))
			{
				fprintf(fptr, "%d. ", game_board->mat[i][j].val);
			}
			else
			{
				if (game_board->mat[i][j].val != -1) 
				{
					fprintf(fptr, "%d ", game_board->mat[i][j].val);
				}
				else
				{
					fprintf(fptr, "0 ");
				}
			}
		}
		fprintf(fptr, "\n");
	}
	fclose(fptr);
}

void markedAllBoard(puzzle* game_board)
{
	int i, j;
	for (i = 0; i < game_board->size; i++)
	{
		for (j = 0; j<game_board->size; j++)
		{
			if (game_board->mat[i][j].val == -1)
			{
				game_board->mat[i][j].marked = false;
			}
			else {
				if (!legalNum(game_board, game_board->mat[i][j].val, i, j,false))
				{
					game_board->mat[i][j].marked = true;
				}
			}
	
		}
	}
}

int hash_command(char* command)
{
	if (strcmp(command, "solve") == 0)
	{
		return e_solve;
	}
	else if (strcmp(command, "edit") == 0)
	{
		return e_edit;
	}
	else if (strcmp(command, "mark_errors") == 0)
	{
		return e_mark_errors;
	}
	else if (strcmp(command, "print_board") == 0)
	{
		return e_print_board;
	}
	else if (strcmp(command, "set") == 0)
	{
		return e_set;
	}
	else if (strcmp(command, "validate") == 0)
	{
		return e_validate;
	}
	else if (strcmp(command, "guess") == 0)
	{
		return e_guess;
	}
	else if (strcmp(command, "generate") == 0)
	{
		return e_generate;
	}
	else if (strcmp(command, "undo") == 0)
	{
		return e_undo;
	}
	else if (strcmp(command, "redo") == 0)
	{
		return e_redo;
	}
	else if (strcmp(command, "save") == 0)
	{
		return e_save;
	}
	else if (strcmp(command, "hint") == 0)
	{
		return e_hint;
	}
	else if (strcmp(command, "guess_hint") == 0)
	{
		return e_guess_hint;
	}
	else if (strcmp(command, "num_solutions") == 0)
	{
		return e_num_solutions;
	}
	else if (strcmp(command, "autofill") == 0)
	{
		return e_autofill;
	}
	else if (strcmp(command, "reset") == 0)
	{
		return e_reset;
	}
	else if (strcmp(command, "exit") == 0)
	{
		return e_exit_game;
	}
	return e_none;
}

bool check_mode(bool init, bool edit, bool solve, int mode)
{
	switch (mode)
	{
	case 0:
		if (init)
			return true;
		break;
	case 1:
		if (edit)
			return true;
		break;
	case 2:
		if (solve)
			return true;
		break;
	}
	printNotRightMode(init,edit,solve);
	return false;
}

bool isNum(char ch)
{
	return (ch >= '0' && ch <= '9');
}

bool isInt(char* str)
{
	if (!str)
		return false;
	while (*str != 0)
	{
		if (!isNum(*str))
		{
			return false;
		}
		str++;
	}
	return true;
}

bool isFloat(char* str)
{
	int point = 0, before = 0, after = 0;
	if (!str)
		return false;
	while (*str != 0)
	{
		if (!isNum(*str))
		{
			if (*str != '.')
			{
				return false;
			}
			else
			{
				point++;
				if (point > 1)
				{
					return false;
				}
			}
		}
		else
		{
			if (point == 0)
			{
				before++;
			}
			else
			{
				after++;
			}
		}
		str++;
	}
	if (point == 1 && (before == 0 || after == 0))
	{
		return false;
	}
	return true;
}
