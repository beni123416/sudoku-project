#ifndef SOLVER_H
#define SOLVER_H
#include <stdlib.h>
#include <stdio.h>
#include <gurobi_c.h>
#include "Generate.h"
#include "LogicAux.h"
/*---------------------------------General Description---------------------------------*/

/**
 * Solver module Summary
 *
 * A container thar represents the sudoko puzzle solution.
 * It will be used to solve the board via the ILP function, make guesses via the LP function
 * and check the number of solution the board has using exhaustive backtracking.
 *
 * LP                     - Checks a possible guesses and their probs for each cell in the board.
 * IL                     - Solves the board using the ILP we saw on class.
 * FullCells              - Checks how many full cells there are in the board.
 * init_matVars           - Creates a 3 dim matrix when the dim is size (a param of the board).
 * find_matVars           - Fills the 3 dim matrix with numbers ascending order starting from 0.
 * numOfVars              - Checks how many vars there are in the 3 dim matrix
 * numOfCellsVars         - Checks how many vars will be created for each cell for ILP and LP.
 * numOfColsVars          - Checks how many vars will be created for each col in board for ILP and LP.
 * numOfRolsVars          - Checks how many vars will be created for each row in board for ILP and LP.
 * numOfCBlockVars        - Checks how many vars will be created for each block in board for ILP and LP.
 * setSol                 - Sets the solution from the ILP of cell in row x and col y in the board.
 * backTracking_exh       - Checks how many solutions there are for the board.
 * emptyCell              - Returns an array with the row and col of the first empty cell we encounter in the board.
 * finishVal              - Checks if there are any more possible legal values that can be used in a specific cell.
 * freeBackTrack          - Frees the dynamic allocated memory that was used in the backtracking.
 * freeILPAndLP           - Frees the memory that was allocated in the ILP and LP by the Gurobi.
 */
/*---------------------------------Commands---------------------------------*/

/**
 * The function uses a 3 dim matrix that stores all the possible values that can be used for each cell
 * and finds the probs of each value to be part of a legal solution of the board.
 * The objective function contains all the possible values of each cell as vars and each cell gets a random weight
 * between 1 to size. We choose this function since we want an equal distribution for each possible legal value we can
 * set on the cell to get a solution (since we don't want to prefer any one value on the other).
 * We are using the constraints that a cell can get only 1 legal value, each row, col and block should contain only
 * one value from 1 to size.
 * The return value is an int that showcases in what part of the LP func we failed for example failing in creating the
 * gurobi  environment is 3 .
 * The function also fills a 3 dim mat with the prob of each value for each cell.
 * @param game_board
 * @param prob
 * @return integer
 */
int LP(puzzle* game_board, double*** prob);
/**
 * The function uses a 3 dim matrix that stores all the possible values that can be used for each cell
 * and finds a solution for the board by giving 1 or 0 to each Cell(vars are getting a binary value).
 * If the mat[i][j][k]=1 it means that in the original game board[i][j] =k+1 (the value is part of a legal sol).
 * There is no need for an objective function for the ILP as we saw in class.
 * We are using the constraints that a cell can get only 1 legal value, each row, col and block should contain only
 * one value from 1 to size.
 * The return value is an int that showcases in what part of the ILP func we failed for example failing in creating the
 * gurobi  environment is 3 .
 * The function also fills the game_board with the solution that was given by the ILP.
 * @param game_board
 * @return integer
 */
int ILP(puzzle* game_board);
/**
 * The function checks how many cells are filled by a value different from -1 (means the cell isn't empty)
 * We will return a count that showcase it.
 * @param game_board
 * @return integer
 */
int FullCells(puzzle* game_board);
/**
 * Creates a new 3 dim mat that will be the size of (rows* cols) X (rows* cols) X (rows* cols).
 * It will be used in th ILP and LP .
 * @param game_board
 * @return
 */
int*** init_matVars(puzzle* game_board);
/**
 * The function checks for any legal value for each not filled cell in the current game board.
 * For any legal value we will put a number in ascending order(1,2,3....) in order to map all the possible
 * vars that will be used in the ILP and LP.
 * It checks if the value is legal using the legalNum function.
 * @param game_board
 * @param ligalVars
 */
void find_matVars(puzzle* game_board, int*** ligalVars);
/**
 *  Returns the number of the vars that will be used in ILP and LP.
 *  It does so by iterating on the 3 dim mat and checking for any value different from -1.
 * @param ligalVars
 * @param size
 * @return integer
 */
int numOfVars(int*** ligalVars, int size);
/**
 * The function checks the number of vars for each cell that will be part of a future constraint in ILP and LP.
 * It return value is the number of vars described above.
 * @param ligalVars
 * @param size
 * @param i
 * @param j
 * @return integer
 */
int numOfCellsVars(int*** ligalVars, int size, int i, int j);
/**
 * The function checks the number of vars for each row in the board that will be part of a future constraint in ILP and LP.
 * It return value is the number of vars described above.
 * @param ligalVars
 * @param size
 * @param i
 * @param k
 * @return integer
 */
int numOfRowsVars(int*** ligalVars, int size, int i, int k);
/**
 * The function checks the number of vars for each col in the board that will be part of a future constraint in ILP and LP.
 * It return value is the number of vars described above.
 * @param ligalVars
 * @param size
 * @param j
 * @param k
 * @return integer
 */
int numOfColsVars(int*** ligalVars, int size, int j, int k);
/**
 * The function checks the number of vars for each block in the board that will be part of a future constraint in ILP and LP.
 * It return value is the number of vars described above.
 * @param ligalVars
 * @param n
 * @param m
 * @param iStart
 * @param jStart
 * @param k
 * @return integer
 */
int numOfBlockVars(int*** ligalVars, int n, int m, int iStart, int jStart, int k);
/**
 * The function sets the value of the solution from the ILP in the board.
 * it will set the value k+1 (since we used the values from 0 to size-1).
 * @param game_board
 * @param i
 * @param j
 * @param k
 */
void setSol(puzzle* game_board, int i, int j, int k);
/**
 * The function checks how many possible solution the current board has by using the backtracking algo.
 * After solving the board once the function won't terminate it will go each time  for the last empty cell
 * and will check if there are any more possible values that can lead to a valid solution of the board.
 * In order to do so we are using a stack that each item there contains an array of the index of the row,col
 * and the last value that was used in the previous solution.
 * We will use a 3 dim mat that will contain all the possible legal values for each cell in order to remember it
 * for any future backtracking we will make.
 * The return value will be a count that represents the number of solution we found .
 * @param game_board
 * @return integer.
 */
int backTracking_exh(puzzle* game_board);
/**
 * The function finds an empty cell given the @inv that empty cell	 have the value -1.
 * the function will save the index of the row and the col of the cell in an array with the size of 2
 * if the matrix is full the array will be filled with -1 in both places
 * @param game_board
 * @return array of ints
 */
int* emptyCell(puzzle* game_board);
/**
 * Checks if there are any more possible legal values that can be set in board[i][j] while iterating over
 * the 3 dim mat - if the value k+1 is legal and mat[i][j][k]!=0 we will return false, else true.
 * @param game_board
 * @param matVars
 * @param i
 * @param j
 * @return boolean value
 */
bool finishVal(puzzle* game_board, int*** matVars, int i, int j);
/**
 * The function frees all the memory that was allocated while using the backTracking_exh.
 * It frees the 3 dim mat, the stack and more.
 * @param index
 * @param fullCell
 * @param matVars
 * @param stack
 * @param size
 */
void freeBackTrack(int* index, int* fullCell, int*** matVars, struct Stack* stack, int size);
/**
 * The function frees all the memory that was allocated while using the ILP or the LP.
 * @param vtype
 * @param arr
 * @param lb
 * @param sol
 * @param size
 */
void freeILPAndLP(char* vtype, int*** arr, double* lb, double* sol, int size);





#endif

