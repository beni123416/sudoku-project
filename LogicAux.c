#include "LogicAux.h"


void updateMarked(puzzle* game_board,int x,int y,int z,int save)
{
    updateInColl(game_board, x, y, z,save);
    updateInRows(game_board, x, y,z,save);
    updateInBlock(game_board, x,  y, z,save);
}

void updateInColl(puzzle* game_board,int x, int y, int z,int save)
{
    int i,saveX,saveY;
    int count = 0;
    for (i = 0; i <game_board->size ; i++)
    {
        if (z == 0)
        {
            if (game_board->mat[i][y].val == save)
            {
                count++;
                saveX = i;
                saveY = y;
            }
        }
        else
        {
            if (game_board->mat[i][y].val == z && i!=x)
            {
                game_board->mat[i][y].marked = true;
                game_board->mat[x][y].marked = true;
            }
        }
    }
    if (count == 1)
    {
        if (legalNum(game_board, save, saveX, saveY,false))
        {
            game_board->mat[saveX][saveY].marked = false;
        }
    }
}

void updateInRows(puzzle* game_board, int x, int y, int z,int save)
{
    int count = 0;
    int i, saveX, saveY;
    for (i = 0; i < game_board->size; i++)
    {
        if (z == 0)
        {
            if (game_board->mat[x][i].val == save)
            {
                count++;
                saveX = x;
                saveY = i;
            }
        }
        else
        {
            if (game_board->mat[x][i].val == z && i!=y)
            {
                game_board->mat[x][i].marked = true;
                game_board->mat[x][y].marked = true;
            }
        }
    }
    if (count == 1)
    {
        if (legalNum(game_board, save, saveX, saveY,false))
        {
            game_board->mat[saveX][saveY].marked = false;
        }
    }
}

void updateInBlock(puzzle* game_board, int x, int y, int z,int save)
{
    int i, j, saveX, saveY;
    int count = 0;
    int rows = game_board->rows;
    int cols = game_board->cols;
    int* startBlock = (int*)malloc(2 * sizeof(int));
    if (!startBlock)
    {
        printFuncError("malloc");
        return;
    }
    startBlock[0] = x - (x % rows);
    startBlock[1] = y - (y % cols);
    for (i = startBlock[0]; i < startBlock[0] + rows; i++)
    {
        for (j = startBlock[1]; j < startBlock[1] + cols; j++)
        {
            if (z == 0)
            {
                if (game_board->mat[i][j].val == save )
                {
                    count++;
                    saveX = i;
                    saveY = j;
                }
            }
            else
            {
                if (game_board->mat[i][j].val == z && i != x && j != y)
                {
                    game_board->mat[i][j].marked = true;
                    game_board->mat[x][y].marked = true;
                }
            }
        }
    }
    if (count == 1)
    {
        if ( legalNum(game_board,save, saveX, saveY,false))
        {
            game_board->mat[saveX][saveY].marked = false;
        }
    }
    free(startBlock);
}

void updateList(Game* game, char *order, int x, int y, int before, int after, int index)
{
    move* data = (move*)malloc(sizeof(move));
    if (!data)
    {
        printFuncError("malloc");
        return;
    }
    data->name = order;
    data->x = x;
    data->y = y;
    data->before = before;
    data->after = after;
    data->index = index;
    game->current_move = insertAfter(game->current_move, data);
}

/* -------------------------------------------------------------------------------------*/

bool checkInColl(puzzle* game_board,  int row ,int col, int num, bool forFixed)
{
    int i;
    for (i = 0; i < game_board->size; i++)
    {
        if (game_board->mat[i][col].val == num && i!=row && (!forFixed || game_board->mat[i][col].fixed))
        {
            return true;
        }
    }
    return false;
}

bool checkInRows(puzzle* game_board, int row,int col, int num, bool forFixed)
{
    int i;
    for (i = 0; i < game_board->size; i++)
    {
        if (game_board->mat[row][i].val == num && i!=col && (!forFixed || game_board->mat[row][i].fixed))
        {
            return true;
        }
    }
    return false;
}

bool checkInBlock(puzzle* game_board, int num, int row, int col, bool forFixed)
{
    int i, j;
    int* startBlock = (int*)malloc(2 * sizeof(int));
    if (!startBlock)
    {
        printFuncError("malloc");
        return false;
    }
    startBlock[0] = row - (row % game_board->rows);
    startBlock[1] = col - (col % game_board->cols);
    for (i = startBlock[0]; i < startBlock[0] + game_board->rows; i++)
    {
        for (j = startBlock[1]; j < startBlock[1] + game_board->cols; j++)
        {
            if (game_board->mat[i][j].val == num && i!=row && j!=col && (!forFixed || game_board->mat[i][j].fixed))
            {
                free(startBlock);
                return true;
            }
        }
    }
    free(startBlock);
    return false;
}

bool legalNum(puzzle* game_board, int num, int row, int col, bool forFixed)
{
    if (!checkInColl(game_board,row, col, num, forFixed) && !checkInRows(game_board, row,col, num, forFixed) && !checkInBlock(game_board, num, row, col, forFixed))
    {
        return true;
    }
    return false;
}

bool validBoard(puzzle* game_board, bool forFixed)
{
    int i, j;
    for (i = 0; i < game_board->size; i++)
    {
        for (j = 0; j < game_board->size; j++)
        {
            if (game_board->mat[i][j].val != -1 && !legalNum(game_board, game_board->mat[i][j].val, i, j,forFixed) && (!forFixed || game_board->mat[i][j].fixed))
            {
                return false;
            }
        }
    }
    return true;
}

bool fullBoard(puzzle* game_board)
{
    int i, j;
    for (i = 0; i < game_board->size; i++)
    {
        for (j = 0; j < game_board->size; j++)
        {
            if (game_board->mat[i][j].val == -1)
            {
                return false;
            }
        }
    }
    return true;
}

/* -------------------------------------------------------------------------------------*/

int* randEmptyCell(puzzle* game_board)
{
    int val, upper;
    int i, j, count = 0;
    int** emptyCells;
    int* index = (int*)malloc(sizeof(int) * 2);
    if (!index)
    {
        printFuncError("malloc");
        return NULL;
    }
    emptyCells = (int**)malloc(sizeof(int*) * game_board->size);
    if (!emptyCells)
    {
        printFuncError("malloc");
        free(index);
        return NULL;
    }
    for (i = 0; i < game_board->size; i++)
    {
        emptyCells[i] = (int*)malloc(sizeof(int) * game_board->size);
        if (!emptyCells[i])
        {
            printFuncError("malloc");
            for (j = 0; j < i; j++)
            {
                free(emptyCells[j]);
            }
            free(emptyCells);
            free(index);
            return NULL;
        }
    }
    for (i = 0; i < game_board->size; i++)
    {
        for (j = 0; j < game_board->size; j++)
        {
            if (game_board->mat[i][j].val == -1)
            {
                emptyCells[i][j] = count;
                count++;
            }
            else
            {
                emptyCells[i][j] = -1;
            }
        }
    }
	if (count == 0)
	{
		printf("Not possibile state\n");
		for (i = 0; i < game_board->size; i++)
		{
			free(emptyCells[i]);
		}
		free(emptyCells);
        free(index);
		return NULL;
	}
    upper = count;
    val = (rand() % (upper));
    for (i = 0; i < game_board->size; i++)
    {
        for (j = 0; j < game_board->size; j++)
        {
            if (emptyCells[i][j] == val)
            {
                index[0] = i;
                index[1] = j;
                break;
            }
        }
    }

    for (i = 0; i < game_board->size; i++)
    {
        free(emptyCells[i]);
    }
    free(emptyCells);
    return index;
}

int randValue(puzzle* game_board, int i, int j)
{
    int k, count = 0;
    int* options;
    int random;
    int value;
	if (!game_board || game_board->size <= 0)
	{
        printf("game_board uninitialised");
        return -1;
	}
    options = (int*)malloc(sizeof(int) * game_board->size);
    if (!options)
    {
        printFuncError("malloc");
        return -1;
    }
    for (k = 0; k < game_board->size; k++)
    {
        if (legalNum(game_board, k + 1, i, j,false))
        {
            options[count] = k + 1;
            count++;

        }
    }
    if (count == 0)
    {
        free(options);
        return -1;
    }

    random = rand() % (count);
    value = options[random];
    free(options);
    return value;
}

int* randNotEmptyCell(puzzle * game_board)
{
    int val, upper;
    int i, j, count = 0;
    int** emptyCells;
    int* index = (int*)malloc(sizeof(int) * 2);
    if (!index)
    {
        printFuncError("malloc");
        return NULL;
    }
    emptyCells = (int**)malloc(sizeof(int*) * game_board->size);
    if (!emptyCells)
    {
        printFuncError("malloc");
        free(index);
        return NULL;
    }
    for (i = 0; i < game_board->size; i++)
    {
        emptyCells[i] = (int*)malloc(sizeof(int) * game_board->size);
        if (!emptyCells[i])
        {
            printFuncError("malloc");
            for (j = 0; j < i; j++)
            {
                free(emptyCells[j]);
            }
            free(emptyCells);
            free(index);
            return NULL;
        }
    }
    for (i = 0; i < game_board->size; i++)
    {
        for (j = 0; j < game_board->size; j++)
        {
            if (game_board->mat[i][j].val != -1)
            {
                emptyCells[i][j] = count;
                count++;
            }
            else
            {
                emptyCells[i][j] = -1;
            }
        }
    }	
	if (count == 0)
	{
		printf("Not found empty cell\n");
		for (i = 0; i < game_board->size; i++)
		{
			free(emptyCells[i]);
		}
		free(emptyCells);
        free(index);
		return NULL;
	}
    upper = count;
    val = (rand() % (upper));
    for (i = 0; i < game_board->size; i++)
    {
        for (j = 0; j < game_board->size; j++)
        {
            if (emptyCells[i][j] == val)
            {
                index[0] = i;
                index[1] = j;
                break;
            }
        }
    }

    for (i = 0; i < game_board->size; i++)
    {
        free(emptyCells[i]);
    }
    free(emptyCells);
    return index;
}

int chooseVal(puzzle* game_board, double*** prob, int i, int j, double x)
{
    int k, index, count = 0;
    double val, upper = 0;
    int lower = 0;
    for (k = 0; k < game_board->size; k++)
    {
        if (prob[i][j][k] > x && legalNum(game_board, k + 1, i, j,false))
        {
            count++;
            index = k;
            upper = upper + prob[i][j][k];
        }
        else
        {
            prob[i][j][k] = 0;
        }
    }
    if (count == 0)
    {
        return -1;
    }
    if (count == 1)
    {
		return index + 1;
    }
    val = ((float)rand()/(float)(RAND_MAX)) * upper;
    while (upper > 0)
    {
        for (k = 0; k < game_board->size; k++)
        {
            if (val >= lower && val < lower + prob[i][j][k])
            {
                return k + 1;
            }
            lower = lower + (int)prob[i][j][k];
        }
    }
    return -1;

}

/* -------------------------------------------------------------------------------------*/

void clearCell(int x, int y, int old_val, Game* game)
{
    game->game_board->mat[x][y].val = -1;
    updateMarked(game->game_board, x, y, 0, old_val);
    game->game_board->mat[x][y].marked = false;
}

void ifBoardIsCompleted(Game* game)
{
    if (fullBoard(game->game_board) && game->mode == 2)
    {
        if (validBoard(game->game_board,false))
        {
            printFullBoard();
            game->mode = 0;
			/*freeInsideGame(game);*/
        }
        else
        {
            printFullBoardNotValid();
        }
    }
}

void copyMat(cell** dst, cell** src, int size, bool update, Game* game, int count)
{
    int i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (update)
            {
                updateList(game, "generate", i, j, dst[i][j].val, src[i][j].val, count);
                count++;
            }
            dst[i][j].val = src[i][j].val;
            dst[i][j].fixed = src[i][j].fixed;
            dst[i][j].marked = src[i][j].marked;
        }
    }
}

void freeProb(double*** prob, int size)
{
    int i,j;
    if (!prob)
    {
        return;
    }
    for (i = 0; i < size; i++)
    {
        if (!prob[i])
        {
            continue;
        }
        for (j = 0; j < size; j++)
        {
            if (prob[i][j])
            {
                free(prob[i][j]);
            }
        }
        free(prob[i]);
    }
    free(prob);
    prob = NULL;
}

void freeMat(cell** mat, int size)
{
    int i;
    if (!mat)
    {
        return;
    }
    for (i = 0; i < size; i++)
    {
        if (mat[i])
        {
            free(mat[i]);
            mat[i] = NULL;
        }
    }
    free(mat);
}

void freeList(Game* game)
{
    while (game->head_list != NULL && game->head_list->next!=NULL)
    {
        free(game->head_list->data);
        game->head_list = game->head_list->next;
        if (game->head_list->prev)
        {
            free(game->head_list->prev);
			game->head_list->prev = NULL;
        }
    }
    if (game->head_list != NULL && game->head_list->next == NULL)
    {
        free(game->head_list->data);
        free(game->head_list);
        game->head_list = NULL;
    } 
}

void freeGame(Game* game)
{
    freeInsideGame(game);
    free(game);
    game = NULL;
}

void freeInsideGame(Game* game)
{
    int size;
    if (game->game_board)
    {
        size =game->game_board->size;
        free_game_board(game->game_board);
		game->game_board = NULL;

        if (game->original_mat)
        {
            freeMat(game->original_mat, size);
            game->original_mat = NULL;
        }
    }

    if (game->head_list)
    {
        freeList(game);
        game->head_list = NULL;
    }
}

void free_game_board(puzzle* game_board)
{
    int size;
    if (game_board)
    {
        size = game_board->size;
        freeMat(game_board->mat, size);
		game_board->mat = NULL;
        freeMat(game_board->solMat, size);
		game_board->solMat = NULL;
        free(game_board);
        game_board = NULL;
    }
}

int hash_list_command(char* command)
{
    if (strcmp(command, "set") == 0)
    {
        return l_set;
    }
    else if (strcmp(command, "guess") == 0)
    {
        return l_guess;
    }
    else if (strcmp(command, "generate") == 0)
    {
        return l_generate;
    }
    else if (strcmp(command, "autofill") == 0)
    {
        return l_autofill;
    }
    return l_none;
}

void initInsideGame(Game* game)
{
    cell** original_mat;
    original_mat = initialize(game->game_board->rows, game->game_board->cols);
    copyMat(original_mat, game->game_board->mat, game->game_board->size, false, game, -1);
    game->original_mat = original_mat;
    game->head_list = (struct Node*)malloc(sizeof(struct Node));
    if (!game->head_list)
    {
        printFuncError("malloc");
        return;
    }
    game->head_list->data = (move*)malloc(sizeof(move));
    if (!game->head_list->data)
    {
        printFuncError("malloc");
        return;
    }
    game->head_list->data->index = -1;
    game->head_list->next = game->head_list->prev = NULL;
    game->current_move = game->head_list;
}


