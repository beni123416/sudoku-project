#ifndef GENERATE_H
#define GENERATE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Prints.h"

/*---------------------------------General Description---------------------------------*/

/**
 * Generate module Summary
 *
 * A container thar represents the structs that are used in the game .
 * It will be used to initialize the structs and set basic commands for some of them.
 *
 * createStack            - The function initialize an empty stack.
 * isFull                 - Checks if the stack is full .
 * isEmpty                - Checks if the stack is empty .
 * pushStack              - The function push a new item into the stack.
 * pop                    - The function deletes an item from the top of the stack.
 * init_Game              - Creates the game settings such as the mode, the undo/redo list and the board.
 * initialize             - Creates an empty matrix for the game.
 * init_puzzle            - Creates an empty board for the game and sets the basic settings of it (like rows,cols,marked)
 * insertAfter            - Inserts after a specific node a new node to the list.
 * free_currNode_nexts    - The function frees  the data of the current node and updates it to its next node.
 *
 */
/*---------------------------------Commands and Structs ---------------------------------*/
/**
 *  The struct is used for the linked list of the undo/redo commands.
 *  The struct describes the current move the user made while index is the number of the move.
 *  The struct saves the command name in the name param, the row and col are saved in x and y param
 *  The struct also saves the new value of board[y][x] in after param (if it's used) and the old val in before param.
 */
typedef struct action
{
	char* name;
	int x;
	int y;
	int before;
	int after;
	int index;
}move;
/**
 * The struct defines a cell in a sudoku matrix.
 * Each cell will have three values that will store an int value ,boolean value that will showcase if the cell is fixed.
 * The third value will store the info of the cell is marked or not.
 */
typedef struct cells 
{
	int val;
	bool fixed;
	bool marked;
}cell;

/**
 *  The struct defines a node in the linked list.
 *  it will the current move that the user made via the data param and the pointers to the next and prev of the node.
 */
struct Node
{
	move* data;
	struct Node* next;
	struct Node* prev;
};
/**
 * The struct defines the sudoku board that is used during the game.
 * It will contain a few params like the rows and cols , the size of the board.
 * The valid param will show us if the board is solvable.
 * markErrors param responsible to showcase us the marked cell when the player sets it to 1.
 * We will store the game board in the mat param and the current solution of the board in the solMat param.
 */
typedef struct board
{
	/*FILE* address;*/
	int cols;
	int rows;
	int size;
	bool valid;
	int	markErrors;
	cell** mat;
	cell** solMat;
}puzzle;
/**
 * The struct defines the game itself.
 * It contains the game board, the linked list of the moves that are played by the user and the game mode.
 * The game mode are described below.
 * It also contains the original matrix of the game as was read from the file.
 */
typedef struct GameFlow
{
	puzzle* game_board;
	struct Node* current_move; /* Node** */
	struct Node* head_list;
	cell** original_mat;
	int mode;/* init=0, edit=1, solv=2, else=-1*/
}Game;
/**
 * The struct contains the size of the stack the top value of the stack .
 * It also contains an array that stores the row ,col and the value of the cell for the exhaustive backtracking.
 */
struct Stack {
	int top; /* how many actually inthe stack*/
	int capacity;
	int** arr;
};

/**
 * The function creates a new stack by setting the capacity to be size^3 (since each cell can get size possible vals).
 * Creates a new two dim array for the values that were mentioned above via dynamic memory allocation.
 * If the allocation fails an informative message will be printed.
 * @param size
 * @return stack
 */
struct Stack* createStack(int size);
/**
 * Checks if the stack is full and returns a value of 1 if so, else 0.
 * @param stack
 * @return 1 or 0
 */
int isFull(struct Stack* stack);
/**
 * Checks if the stack is empty if so it will return 1, else 0
 * @param stack
 * @return 1 or 0
 */
int isEmpty(struct Stack* stack);
/**
 * The function pushes a new item into the stack.
 * It will update the top by doing ++stack->top and it will update the cell row,col and value to the stack .
 * @param stack
 * @param i
 * @param j
 * @param k
 */
void pushStack(struct Stack* stack, int i, int j, int k);
/**
 * The function deletes the current top item from the stack by updating the top param of the stack.
 * @param stack
 * @return pointer to the current top
 */
int* pop(struct Stack* stack);
/**
 * The function creates a new sudoku game by setting the board of the game, the linked list of the moves to be empty
 * and the game mode to be 0(init which is the default mode of the game).
 * It creats the game by dynamic memory allocation.
 * If the allocation fails an informative message will be printed.
 * @param game_board
 * @return pointer to the game.
 */
Game* init_Game(puzzle* game_board);
/**
 * The function makes a matrix with the size of (rows*cols )X (rows*cols)
 * The function puts -1 on every cell in the matrix and sets the fixed and marked value to be false by default .
 * @param rows
 * @param cols
 * @return pointer to the matrix
 */
cell** initialize(int rows, int cols);
/**
 * The function creates the sudoku game board by calling initialize with the params rows and cols.
 * It updates the params rows, cols,size and markedErrors.
 * It gives a default value of true to the valid param since the board is empty and solvable and sets the solMat
 * to be an empty mat until the ILP function will be used by the user.
 * @param rows
 * @param cols
 * @return pointer to the board
 */
puzzle* init_puzzle(int rows, int cols);
/**
 * The function checks if curr_node is null if so the list isn't valid (since it should contain the head at least).
 * It creates a new node and updates its params accordingly.
 * It also updates the prev and next pointers of the new node according to the index.
 * if its the first move in game (index=-1) or something else.
 * @param curr_node
 * @param new_data
 * @return pointer to the new node
 */
struct Node* insertAfter(struct Node* curr_node, move* new_data);
/**
 * The function frees the memory of the current node and any node after him from the system.
 * @param curr_node
 */
void free_currNode_nexts(struct Node* curr_node);

void freeStack(struct Stack* stack);

#endif
