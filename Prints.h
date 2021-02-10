#ifndef PRINTS_H
#define PRINTS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Generate.h"
/*---------------------------------General Description---------------------------------*/
/**
 * Prints module Summary
 *
 * A container thar represents the printing messages that are used in the game.
 * It will be used to print informative messages and errors to the user.
 *
 * Please note the we won't describe here what each function does since the function just prints a message to the user
 * and it will repeating the same thing.
 * We will describe each command briefly bellow.
 *
 */

/*---------------------------------Commands---------------------------------*/
/**
 * Prints an error that are no more commands to redo in the linked list.
 */
void printNoMoreRedo();
/**
 * Prints a welcoming message to the user and notes them that they are in an init mode.
 */
void printWelcome();
/**
 *  Prints an error that they are no more previous commands to undo.
 */
void printNoMoreUndo();
/**
 * Prints an error that the row/col x should be between 1 to size.
 * @param x
 * @param size
 */
void ParamError(int x, int size);
/**
 * Prints an error that the value of the cell should be between 0 to size .
 * @param z
 * @param size
 */
void setParamZError(int z, int size);
/**
 * Prints an error that the cell is fixed and we can't change it.
 */
void printFixedError();
/**
 * Prints the user a message that the puzzle was solved
 */
void printFullBoard();
/**
 * Prints a message that the func failed and exits the game.
 * @param func
 */
void printFuncError(char* func);
/**
 * Prints an error to the user that the command that was typed is incorrect .
 */
void printInvalidCommand();
/**
 * Prints an error message to the user that depicts that the number of the params entered is wrong .
 * It also describes how many params the user must enter.
 * @param num
 */
void printWrongParams(int num);
/**
 * Prints an error that the file doesn't exist.
 */
void printFileError();
/**
 * Prints an error of the format of the file to the user - it contains digits lesser than 0.
 */
void printIllegalFormat();
/**
 * Prints an error to the user that the file contains numbers that are larger than size of the board (which is illegal).
 * @param size
 */
void printRangeError(int size);
/**
 * Prints an error that the single digits shouldn't contain a zero before them.
 */
void printSingleDigitError();
/**
 * Prints an error that the cell can contains more than two digits or more than one dot in the file.
 */
void printTooManyDigits();
/**
 * Prints an error that the file is too short .
 */
void printShortError();
/**
 * Prints an error that the file is too long.
 */
void printLongError();
/**
 * Prints an error that the rows and cols that was read from the file aren't digits.
 */
void printNotDigitError();
/**
 * Prints an error that the rows and cols should be between 0 to 5 as been told by moshe.
 */
void printTooBig();
/**
 * Prints an error that there are too many numbers in line of the file
 */
void printTooManyInALine();
/**
 * Prints an error that there are not enough numbers in a line of the file
 */
void printNotEnoughInLine();
/**
 * Prints a message that the board is valid -means it in the correct form as been described by the game rules.
 */
void printValidBoard();
/**
 * Prints a message that the board is invalid -means it in the incorrect form as been described by the game rules.
 */
void printInvalidBoard();
/**
 * Prints a message to the user that he tried to use function not in the right mode and what mode the func is avaliable.
 * @param init
 * @param edit
 * @param solve
 */
void printNotRightMode(bool init, bool edit, bool solve);
/**
 * Prints an error that the cell already has a value .
 */
void printContainVal();
/**
 * Prints the hint value of the cell that the user requested.
 * @param val
 */
void printValHint(int val);
/**
 * Prints the number of solutions of the board to the user.
 * @param count
 */
void printNumOfSolve(int count);
/**
 * Prints an informative message that mat[i][j] was filled with value to the user.
 * @param i
 * @param j
 * @param num
 */
void printAutoFill(int i, int j, int  num);
/**
 * Prints an error that threshold is invalid and should be between 0 to 1.
 */
void printInvalidThreshold();
/**
 * Prints an error that they aren't enough empty cells in the board.
 */
void printNotEnoughEmptyCells();
/**
 * Prints an error that the board is full but erroneous.
 */
void printFullBoardNotValid();
/**
 * Prints a message the board is solvable.
 */
void printSolvableBoard();
/**
 * Prints a message that the board isn't solvable.
 */
void printUnsolvableBoard();
/**
 * Prints an error that the generation has failed.
 */
void printGenerationFailed();
/**
 * Prints all the legal values and their score via the LP function to the user.
 * @param size
 * @param x
 * @param y
 * @param prob
 */
void printAllLegalValues(int size, int x, int y, double*** prob);
/**
 * Prints an exiting message to the user.
 */
void printExitGame();
/**
 * Prints an error to the user that too many chars were entered in the command line and that you can enter 256 chars
 * at most.
 */
void printTooManyCharacters();
/**
 * Asks the user to enter a command.
 */
void printPrompt();
/**
 * Prints an error that the params can be only integers.
 */
void printNotIntParam();
/**
 * Prints an error that the parameter can be only 0 or 1.
 */
void printParam01();
/**
 * Prints an error that the parameter can be only float.
 */
void printNotFloatParam();
/**
 * Prints an information about the undo or redo move - like which cell was changed and what value did the cell contains now 
 * @param x
 * @param y
 * @param z
 * @param 
 */
 void printUndoRedo(int x, int y, int z);
/**
 * Prints the bounds of a value property
 * @param of_value
 * @param min
 * @param max
 * @param 
 */
 void printValueBetween(char* of_value, int min, int max);

#endif
