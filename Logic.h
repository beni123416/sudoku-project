#ifndef LOGIC_H
#define LOGIC_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Generate.h"
#include "Prints.h"
#include "Solver.h"
#include "Parser.h"
#include "LogicAux.h"


/*---------------------------------General Description---------------------------------*/

/**
 * Logic module Summary
 *
 * A container thar represents the logic that is used in the game.
 * It will be used to execute all the commands that the user can do while playing the game.
 * This module is the heart of the game.
 *
 * solve                  - Starts a puzzle in a solve mode.
 * edit	                  - Starts a puzzle in an edit mode.
 * mark_errors            - Sets the mark parameter of the game to 1 or 0.
 * print_board            - The function prints the board to the user.
 * set                    - The function sets in board[y][x] the value z .
 * validate               - The function checks if the board is solvable.
 * guess		          - The function fills the cells with score greater than X.
 * generate               - Generates a puzzle by filling X cells and then deleting Y of the full board.
 * undo                   - The function undo's the previous command by the user
 * redo                   - The function does a previous undone command by the user.
 * save                   - The function saves the game board to a new file.
 * hint                   - The function gives a hint of a solution of a single cell to the user.
 * guess_hint             - The function gives a possible solution of a single cell to the user.
 * num_solutions          - The function shows how many possible solutions there are to the current board.
 * autofill               - The function automatically fill "obvious" values of cells.
 * reset                  - The function resets all the game to its beginning state.
 * exit                   - The function makes a clean exit for the game.
 * redoCell               - The function sets the cell value in coordination to the prev undo move.
 * undoCell               - The functions sets the cell value in coordination to the previous value of the cell.

 *
 */
/*---------------------------------Commands and Structs ---------------------------------*/



/**
 * The function opens a game in a solve mode.
 * It gets a path the board will be created from the file path.
 * If the board is erroneous we will print an informative message to the user.
 * We will print the board at the end of the process (if everything is ok).
 * @param game
 * @param path
 */
void solve(Game* game, char* path);
/**
 * The function opens a game in an edit mode.
 * If it gets a path the board will be created from the file path, else we will create an empty 9X9 board.
 * If the board is erroneous we will print an informative message to the user.
 * We will print the board at the end of the process (if everything is ok).
 * @param game
 * @param path
 */
void edit(Game* game, char* path);
/**
 * Changes the value of the mark param of the game to 1 or 0.
 * If it's 1, while printing the board the marked cell will be showcased, else they won't.
 * @param game_board
 * @param x
 */
void mark_errors(puzzle* game_board, int x);
/**
 * The function prints the board as been described by the game instructions.
 * @param game_board
 */
void print_board(Game* game_board);
void print_board_testing(puzzle* game_board);
/**
 * The function is used when the commend "set" is written by the user.
 * If the board[x][y].fixed = true we will print an error since we cant delete or set new numbers in fixed cell.
 * the function will set in board[x][y] z which is a num between 1 to size if its legal by using the func legalNum
 * if z=0 so we will delete the number that is placed in board[x-1][y-1].
 * if the board is full after placing z we will print the  user a winning message.
 * In addition we will update the marked param of each cell after any set command that was done by using the funcs
 * updateInRow, updateInColl and updateInBlock that does it for us.
 * We also save the params of the move in the linked list of undo/redo.
 * @param x
 * @param y
 * @param z
 * @param game
 * @param update
 */
void set(int x, int y, int z, Game* game, bool update);
/**
 * The function checks if the board is solvable by running the ILP prog.
 * We will update the user regrading the result of the ILP by printing an informative message.
 * @param game
 */
void validate(Game* game);
/**
 * The function guesses a solution via the LP prog with threshold X.
 * The function will fill all cells with values greater than X, and if several values are possible for the same cell
 * we will choose randomly among them.
 * If the board is invalid we will print an informative message to the user.
 * We also save the params of the move in the linked list of undo/redo.
 * @param game
 * @param x
 */
void guess(Game* game, double x);
/**
 *  The function generates a puzzle for the user by filling x random empty cells with legal values via the ILP
 *  and then clearing all the board but Y random cells.
 *  If one of the x cells doesn't have a legal value or the board isn't solvable we will reset the game to its original
 *  state and repeat it again.
 *  After 1000 tries we will print an error message to the user.
 * We also save the params of the move in the linked list of undo/redo.
 * @param game
 * @param x
 * @param y
 */
void generate(Game* game, int x, int y);
/**
 * The function undo's a previous move done by the user.
 * In order to fulfil it we use the linked list that contains the moves the user did .
 * We use the undoCell function that sets the value before the change that was made by the user.
 * If there is no move to undo we print an informative error to the user.
 * The user gets a message of the changes that were made.
 * @param game
 */
void undo(Game* game);
/**
 * The function redo's a previous move done by the user.
 * In order to fulfil it we use the linked list that contains the moves the user did .
 * We also set the pointer of the list to the next of the current Node and update the board.
 * If there is no move to redo we print an informative error to the user.
 * The user gets a message of the changes that were made.
 * @param game
 */
void redo(Game* game);
/**
 * The function gets a full or relative path from the user and creates a new file.
 * The new file will contain the current board of the game .
 * It will be saved according to the rules that were stated.
 * We will not allow to save invalid boards or not solvable boards in edit mode.
 *
 * @param game
 * @param path
 */
void save(Game* game, char* path);
/**
 * Gives a solution for a single cell for the user (the cell is mat[x-1][y-1]).
 * It does it by running the ILP prog on the board and printing the info to the user.
 * If the board isn't solvable or the cell is fixed we will print an error to the user.
 * @param game
 * @param x
 * @param y
 */
void hint(Game* game, int x, int y);
/**
 * Gives the user a possible solution for a single cell (the cell is mat[x-1][y-1]).
 * It does so by running the LP prog and printing all the possible solutions and their probs to the user.
 * If the board isn't solvable or the cell is fixed an error will be printed.
 * @param game
 * @param x
 * @param y
 */
void guess_hint(Game* game, int x, int y);
/**
 * The function runs the backtracking_exh function and prints all the possible solutions for the current game board.
 * If the board is invalid or unsolvable we will print an error to the user.
 * @param game
 */
void num_solutions(Game* game);
/**
 * Automatically fill "obvious" values – cells which contain a single legal value.
 * If the board is erroneous we will print an informative message to the user.
 * The function does it by  creating a new mat that will contain only the cells which had an obvious single value
 * in the original game board.
 * @param game
 */
void autofill(Game* game);
/**
 * Undo all moves, reverting the board to its original loaded state.
 * We won't delete the list and create a new one we will move the pointer to the head of the list at the end
 * of the process.
 * We saved the original mat in the game struct so we used it to revert the game to its beginning so it basically acts
 * like doing undo all over the linked list until the head.
 * @param game
 */
void reset(Game* game);
/**
 *  We make a clean exit from the game by freeing all the allocated memory by freeGame func
 *  and print a message to the user.
 * @param game
 */
void exitGame(Game* game);
/**
 * This function is an auxiliary function that is used for the redo command.
 * The function uses the set function in order to set the value of the cell that was described in the linked list
 * to its value that was previously undone by the user.
 * @param x
 * @param y
 * @param after
 * @param game
 */
void redoCell(int x, int y, int after, Game* game);
/**
 * This function is an auxiliary function that is used for the undo command.
 * The function uses the set function in order to set the value of the cell that was described in the linked list
 * to its value before the move in the last node of the linked list was made.
 * @param x
 * @param y
 * @param before
 * @param game
 */
void undoCell(int x, int y, int before, Game* game);

#endif
