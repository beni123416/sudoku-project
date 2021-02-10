#ifndef LOGICAUX_H
#define LOGICAUX_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Generate.h"

/*---------------------------------General Description---------------------------------*/

/**
 * LogicAux module Summary
 *
 * A container thar represents the auxiliary functions that are used to preform the logic of the game.
 *
 * updateInColl           - The function updates the marked value of cells in a col.
 * updateInRows            - The function updates the marked value of cells in a row.
 * updateInBlock          - The function updates the marked value of cells in a Block.
 * updatedMarked          - The function updates the marked param for each cell in the game.
 * fullBoard              - Checks if the board is full.
 * legalNum               - Checks if the number is legal in block, row and col.
 * CheckInBlock           - Checks if the number is legal in a block of the board.
 * CheckInColl            - Checks if the number is legal in a col of the board.
 * CheckInRow             - Checks if the number is legal in a row of the board.
 * validBoard             - Checks if the board is erroneous or not.
 * chooseVal              - Chooses a random value from the LP result to a single cell.
 * randEmptyCell          - Returns the indexs of a random empty cell.
 * randValue              - Chooses a random legal option for a single cell.
 * randNotEmptyCell       - Returns the indexs of a random not empty cell.
 * clearCell              - Clears the value of a single not fixed cell .
 * ifBoardIsCompleted     - Checks if the board is completed and prints message to the user.
 * copyMat                - Hardcopies a 2 dim matrix.
 * freeProb               - Free's the matrix that is used for the LP result.
 * freeMat                - Free's a 2 dim matrix.
 * freeGame               - Free's the game memory.
 * freeInsideGame         - Free's all the structs that are used in the game.
 * free_game_board        - Free's the memory of the game_board struct.
 * initInsideGame         - Creates the game we started from again.
 *
 */
/*---------------------------------Commands and Structs ---------------------------------*/

/**
 * The enum members represents the commands that are part of the linked list , it means that can be done while
 * doing the undo and redo commands.
 */
enum list_Commands
{
    l_set,
    l_guess,
    l_generate,
    l_autofill,
    l_none
};

void updateInColl(puzzle* game_board, int x, int y, int z, int save);
/**
 * The function goes over the row that had a change when one of the cell's there changed its value by functions
 * like set,autofill,guess and so on.
 * It will update the marked parameter of each cell in the row according to the rules.
 * @param game_board
 * @param x
 * @param y
 * @param z
 * @param save
 */
void updateInRows(puzzle* game_board, int x, int y, int z, int save);
/**
 * The function goes over the block that had a change when one of the cell's there changed its value by functions
 * like set,autofill,guess and so on.
 * It will update the marked parameter of each cell in the block according to the rules.
 * @param game_board
 * @param x
 * @param y
 * @param z
 * @param save
 */
void updateInBlock(puzzle* game_board, int x, int y, int z, int save);
/**
 * This function is an auxiliary function that is used for every time the user changes the current board.
 * After a change of a single cell in the board by a command that was made by the user we will use this function.
 * This function calls to the 3 updateIn function described above and change the marked parameter of all cells that
 * are part of the cell (that was changed) row, col and block.
 * @param game_board
 * @param x
 * @param y
 * @param z
 * @param save
 */
void updateMarked(puzzle* game_board, int x, int y, int z, int save);
/**
 * The function checks if the board is full by checking if cell in board[i][j]=-1 which shows there is an empty cell.
 * if the board is full we will return true, else false.
 * @param game_board
 * @return boolean value
 */
bool fullBoard(puzzle* game_board);
/**
 * This function updates the list by creating a new node that contains the name of the command that was made by the user
 * the index of the cell, the new value and the old value of the cell
 * The index value is used for the autofill command which showcases us when did it start and when did it finish in
 * the linked list ( every autofill move gets a num in increasing order from 0).
 * @param game
 * @param order
 * @param x
 * @param y
 * @param before
 * @param after
 * @param index
 */
void updateList(Game* game, char* order, int x, int y, int before, int after, int index);
/**
 * This function is an auxiliary function that is used for multiply functions in this module like updateInCol,Row
 * Block and more.
 * The function checks if num is legal in the row, col and block that mat[row][col] belongs to.
 * the function calls the function checkInBlock , checkInRow, checkInColl.
 * if all the functions return the boolean value true the num is legal, else its not legal.
 * @param game_board
 * @param num
 * @param row
 * @param col
 * @param forFixed
 * @return
 */
bool legalNum(puzzle* game_board, int num, int row, int col, bool forFixed);
/**
 * num represents the potential value that will be placed on mat[row][col].
 * each block on the matrix is of the size row X col.
 * the function checks if the number is legal in the block that mat[row][col] belongs to.
 * if num is legal the function will return the boolean value true, else false.
 * @param game_board
 * @param num
 * @param row
 * @param col
 * @param forFixed
 * @return
 */
bool checkInBlock(puzzle* game_board, int num, int row, int col, bool forFixed);
/**
 * num represents the potential value that will be placed on mat[row][col].
 * the function checks if the number is legal in the row of the matrix as been described in the sudoku rules.
 * if its legal the function will return  a boolean value of true , else false.
 * num represents  the potential value that will be placed on mat[row][col].
 * @param game_board
 * @param row
 * @param col
 * @param num
 * @param forFixed
 * @return
 */
bool checkInRows(puzzle* game_board, int row, int col, int num, bool forFixed);
/**
 * num represents the potential value that will be placed on mat[row][col].
 * the function checks if the number is legal in the row of the matrix as been described in the sudoku rules.
 * if its legal the function will return  a boolean value of true , else false.
 * num represents  the potential value that will be placed on mat[row][col].
 * @param game_board
 * @param row
 * @param col
 * @param num
 * @param forFixed
 * @return
 */
bool checkInColl(puzzle* game_board, int row, int col, int num, bool forFixed);
/**
 * This function checks if the board is erroneous as been described by the rules of the game.
 * For example if the board contains value the legalNum return false on it, it means the board is erroneous
 * and we will return false to the user.
 * We are doing it by going over only the not fixed cells in the game, we use the forFixed boolean value to do so.
 * If the board is ok we will return true to the user, means the board isn't erroneous.
 * @param game_board
 * @param forFixed
 * @return boolean value.
 */
bool validBoard(puzzle* game_board, bool forFixed);
/**
 * This function is an auxiliary function that is used in the guess function command.
 * We go over the 3 dim matrix we get from the LP prog, and choose randomly a value from all the possible
 * ones that their probability is larger than x as been told in the rules.
 * We do it by maintaining an upper and a lower bound and updating them until we will randomize
 * value with prob larger than x.
 * If we doesn't have a value with prob larger than x or all the probs are 0 we will return -1 else we
 * will return the possible value to the user.
 * @param game_board
 * @param prob
 * @param i
 * @param j
 * @param x
 * @return integer
 */
int chooseVal(puzzle* game_board, double*** prob, int i, int j, double x);
/**
 * We use this function in the generate function.
 * The function returns an array that contains the index of the row and col of a random empty cell.
 * We do it by maintaining an upper bound and updating it until we will randomize an empty cell.
 * We also use a new matrix that contains a value for each cell that is empty, the number are given in increasing order
 * (0,1,2,3...) and we randomize a val from 0 to count
 * We know that its an empty cell if the value of mat [i][j] fulfill- mat[i][j]=va .
 * @param game_board
 * @return an array of integer.
 */
int* randEmptyCell(puzzle* game_board);
/**
 * This function is used in the generate function.
 * We randomize a legal value from the list of all the possible legal value of a single cell and return it.
 * If there are no legal value that can be set in the cell we will return -1 which implicates that the cell
 * remains empty.
 * @param game_board
 * @param i
 * @param j
 * @return integer
 */
int randValue(puzzle* game_board, int i, int j);
/**
 * We use this function in the generate function.
 * The function returns an array that contains the index of the row and col of a random not empty cell.
 * We do it by maintaining an upper bound and updating it until we will randomize a not empty cell.
 * We also use a new matrix that contains a value for each cell that is not empty, the number are given in increasing
 *  order(0,1,2,3...) and we randomize a val from 0 to count
 * We know that its not an empty cell if the value of mat[i][j] fulfills - mat[i][j]=val
 * @param game_board
 * @return an array of integer
 */
int* randNotEmptyCell(puzzle* game_board);
/**
 * We use this function in the set command.
 * The function clears an empty cell by setting its value to -1 and updating the marked parameter
 * of each cell that is part of the row,col or block of the changed cell.
 * @param x
 * @param y
 * @param old_val
 * @param game
 */
void clearCell(int x, int y, int old_val, Game* game);
/**
 * We use this function is the solve command.
 * Checks if the board is completed and valid ,means the solution is legal.
 * If so we will change the mode of the game to 0(init mode), free the current game and print a message to the user.
 * @param game
 */
void ifBoardIsCompleted(Game* game);
/**
 * The function hard copies the matrix/board of sudoku using a dynamic memory allocation.
 * for each cell in the copied matrix we set matrix[i][j] val and fixed as it is in mat[i][j]
 * @param dst
 * @param src
 * @param size
 * @param update
 * @param game
 * @param count
 */
void copyMat(cell** dst, cell** src, int size, bool update, Game* game, int count);
/**
 * The function frees the 3 dim matrix that is created  by the LP prog.
 * @param prob
 * @param size
 */
void freeProb(double*** prob, int size);
/**
 * The function frees the allocated memory of a 2 dim matrix.
 * @param mat
 * @param size
 */
void freeMat(cell** mat,int size);
/**
 * The function frees the game struct by calling the function freeInsideGame and then prints an exit message.
 * @param game
 */
void freeGame(Game* game);
/**
 * The function frees all the structs that are used by the game itself.
 * In order to  ree the game board itself we call the function free_game_board.
 * @param game
 */
void freeInsideGame(Game* game);
/**
 * This function frees the game_board struct by calling the function freeMat which frees
 * all the memory that was allocated from the matrix that are used as the board of the game.
 * @param game_board
 */
void free_game_board(puzzle* game_board);
/**
 * We use this function in the solve and edit commands, since we want to save the game's original matrix
 * and create a new linked list for the undo/redo commands.
 * In the beginning of the game when we go to solve or edit mode the function is called.
 * The function updates the games original matrix parameter by hardcopying the game board, it uses the copyMat func.
 * It also creates a new linked list that will be used during the game to store the commands that
 * can be undone or redone .
 *
 * @param game
 */
void initInsideGame(Game* game);
/**
 * The function translates a command in form of a string to its enum member.
 * It return the enum member of the equal command.
 * @param command
 * @return enum member.
 */
int hash_list_command(char* command);
#endif
