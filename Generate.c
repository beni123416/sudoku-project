#include "Generate.h"
#include "Logic.h"
#include "Parser.h"
#include "Prints.h"
#include "Solver.h"

cell** initialize(int rows, int cols)
{
	int i, j, k, size = rows * cols;
	cell** mat = (cell * *)malloc(size * sizeof(cell*));
	if (!mat)
	{
		printFuncError("malloc");
		return NULL;
	}
	for (i = 0; i < size; i++)
	{
		mat[i] = (cell*)malloc(size * sizeof(cell));
		if (!mat[i])
		{
			printFuncError("malloc");
			return NULL;
		}
	}
	for (j = 0; j < size; j++)
	{
		for (k = 0; k < size; k++)
		{
			mat[j][k].val = -1;
			mat[j][k].fixed = false;
			mat[j][k].marked = false;
		}
	}
	return mat;
}

Game* init_Game(puzzle* game_board)
{
	Game* game = (Game*)malloc(sizeof(Game));
	if (!game)
	{
		printFuncError("malloc");
		return NULL;
	}
	game->game_board = game_board;
	game->original_mat = NULL;
	game->head_list = (struct Node*)malloc(sizeof(struct Node));
	if (!game->head_list)
	{
		printFuncError("malloc");
		return NULL;
	}
	game->head_list->data = (move*)malloc(sizeof(move));
	if (!game->head_list->data)
	{
		printFuncError("malloc");
		return NULL;
	}
	game->head_list->data->index = -1;
	game->head_list->next = game->head_list->prev = NULL;
	game->current_move = game->head_list;
	game->mode = 0;
	return game;
}

puzzle* init_puzzle(int rows, int cols)
{
	cell** mat = initialize(rows, cols);
	cell** solMat= initialize(rows, cols);
	puzzle* game_board = (puzzle*)malloc(sizeof(puzzle));
	if (!game_board)
	{
		printFuncError("malloc");
		return NULL;
	}
	game_board->cols = cols;
	game_board->rows = rows;
	game_board->size = rows * cols;
	game_board->mat = mat;
	game_board->solMat = solMat;
	game_board->markErrors = 1;
	game_board->valid = true;
	return game_board;
}

/*-------------------------------------------------------------------------------------------------------------*/

struct Node* insertAfter(struct Node* curr_node, move* new_data)
{
	struct Node *new_node;
	if (curr_node == NULL)
	{
		printf("error");
		return NULL;
	}
	new_node = (struct Node*)malloc(sizeof(struct Node));
	if (!new_node)
	{
		printFuncError("malloc");
		return NULL;
	}
	new_node->data = new_data;
	new_node->next = NULL;
	new_node->prev = curr_node;
	free_currNode_nexts(curr_node);
	curr_node->next = new_node;

	return new_node;
	
}

void free_currNode_nexts(struct Node* curr_node)
{
	struct Node* free_head;
	if (curr_node == NULL)
	{
		return;
	}
	free_head=curr_node->next;
	if (free_head == NULL)
	{
		return;
	}
	curr_node->next = NULL;
	free_head->prev = NULL;
	curr_node = free_head;
	while (curr_node != NULL && curr_node->next != NULL)
	{
		free(curr_node->data);
		curr_node = curr_node->next;
		if (curr_node->prev)
		{
			free(curr_node->prev);
		}
	}
	if (curr_node != NULL && curr_node->next == NULL)
	{
		free(curr_node->data);
		free(curr_node);
	}
}

/*-----------------------------------------------------------------------------------------------------------*/

struct Stack* createStack(int size)
{
	int i;
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
	if (!stack)
	{
		printFuncError("malloc");
		return NULL;
	}
	stack->capacity = size;
	stack->top = 0;
	stack->arr = (int**)malloc(stack->capacity * sizeof(int*));
	if (!stack->arr || stack->capacity <= 0)
	{
		printFuncError("malloc");
		return NULL;
	}
	for (i = 0; i < stack->capacity; i++)
	{
		stack->arr[i] = (int*)malloc(sizeof(int) * 3);
		if (!stack->arr[i])
		{
			printFuncError("malloc");
			return NULL;
		}
	}

	return stack;
}

int isFull(struct Stack* stack)
{
	return stack->top == stack->capacity;
}

int isEmpty(struct Stack* stack)
{
	return stack->top == 0;
}

void pushStack(struct Stack* stack, int i, int j, int k)
{
	if (isFull(stack))
		return;
	stack->arr[stack->top][0] = i;
	stack->arr[stack->top][1] = j;
	stack->arr[stack->top][2] = k;
	++(stack->top);
}

int* pop(struct Stack* stack)
{
	if (isEmpty(stack))
		return NULL;
	--(stack->top);
	return stack->arr[stack->top];
}

void freeStack(struct Stack* stack)
{
	int i;
	if (!stack)
	{
		return;
	}
	if (!stack->arr || stack->capacity <= 0)
	{
		return;
	}
	for (i = 0; i < stack->capacity; i++)
	{
		if (stack->arr[i])
		{
			free(stack->arr[i]);
		}
	}
	free(stack->arr);
	free(stack);
}