#ifndef	PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Generate.h"
#include <string.h>


/*---------------------------------General Description---------------------------------*/

/**
 * Parser module Summary
 *
 * A container thar represents the input that is given by the user.
 * It will be used to read commends and interpret them by the user.
 *
 * readComment              - Reads the comments by the user.
 * fileToPuzzle             - Reads a file and creates a sudoku board from it.
 * puzzleToFile             - Creates a file containing the sudoku board.
 * markedAllBoard           - Sets the marked param for each cell in the board.
 * fileToCells              - Fills the cell in the matrix with the num that was read.
 * readMN                   - Reads the row and col from the file
 * hash_command             - Reads the command that is typed by the user and returns the correct enum.
 * check_mode               - Reads the mode from the user .
 * isNum                    - Checks if the char is a num between 0-9 or not.
 * isInt                    - Checks if the string entered by the user is an int.
 * isFloat                  - Checks if the string entered by the user is float.
 * IfFix1                   - Checks if the num in the file is between 0-9 and fixed .
 * ifFix2                   - Checks if the num in the file is larger than 9 and fixed .
 * IfNotFix1                - Checks if the num in the file is between 0-9 and not fixed .
 * IfNotFix2                - Checks if the num in the file is larger than 9 and not fixed .
 *
 */

/*---------------------------------Commands---------------------------------*/
/**
 *  the enum is used to determine which command did the user used by creating a suitable member
 *   for each command in the enum.
 *   if the command, that is typed by the user, isn't correct it will be e_none in the enum.
 */
enum Commands
{
	e_solve,
	e_edit,
	e_mark_errors,
	e_print_board,
	e_set,
	e_validate,
	e_guess,
	e_generate,
	e_undo,
	e_redo,
	e_save,
	e_hint,
	e_guess_hint,
	e_num_solutions,
	e_autofill,
	e_reset,
	e_exit_game,
	e_none
};
/**
 * The function reads the command by the user using strtok and fgets that are standart functions in string.h library .
 * For each legal comment that was described in the assignment the function will send the relevant parmas,
 * for the suitable comment function using switch-case and the enum we described above.
 * The function allows only command with 256 characters beyond that it will print an informative message to the user.
 * if the comment is invalid we will get to the last case e_none and print an informative message to the user.
 * if the comment is valid but we are not in a valid mode the function will print an informative message to the user.
 * @param game
 */
void readCommand(Game* game);
/**
 * The function gets the path (relative or full) from the user and create a sudoku from it.
 * The function ignores the first line where the rows and cols of the board are written .
 * The function reads the file and creates the board by transforming the string into number using the function
 * IfFix1,IfFix2, IfNotFix1,IfNotFix2 and fileToCell(it uses fgets, strtok and strchr for the process).
 * It the file is to short or to long meaning it doesn't contain the exact amount of numbers as the size^2 of the matrix
 * the function will terminate return false and print and informative message to the user.
 * If the line contains to much numbers or to little, meaning the amount of numbers in the line isn't size of the mat
 * the function will terminate return false and print and informative message to the user.
 * @param path
 * @param game_board
 * @return booleam value
 */
bool fileToPuzzle(char* path, puzzle* game_board);
/**
 * The function gets the path of the new file that will be created to save the board from the user.
 * The function writes the matrix rows and cols to the first line and writes each line in a new line in the file.
 * Fixed cells will be written as ".num" and regular cells as "num".
 * We use fprintf function for the process as been taught in class.
 * @param game_board
 * @param path
 * @param mode
 */
void puzzleTofile(puzzle* game_board, char* path,int mode);
/**
 * The function checks which cell is marked as been described in the rules of the game.
 *  If its marked we will change the mark bool value of the cell to true else it will remain false.
 * @param game_board
 */
void markedAllBoard(puzzle* game_board);
/**
 * The function reads the dimension of the board, it reads the rows and cols using the path given by the user.
 * If the file doesn't exist or the path is wrong we will print informative message to the user.
 * If in the first fgets we have more than 2 params that we read it means the format is wrong and terminate the prog.
 * If it still in the wrong format and wasn't revealed in this function it will revealed in the fileToPuzzle function .
 * We assume that the largest matrix could be a 5X5 as been told by moshe in the forum, any larger dim of the matrix
 * will be rejected by the program.
 * @param path
 * @return array containing the size of rows and cols of the board.
 */
int* readMN(char* path);
/**
 * Transforms the command that is typed by the user to its equal command in the enum and returns it to the user.
 * If the command is invalid or doesn't exist it will return e_none.
 * @param command
 * @return an enum member
 */
int hash_command(char* command);
/**
 * The function checks the current mode we are in and returns true if the boolean value in the correct param is true
 * it uses switch and case to do so.
 * We use the function to determine if the command is valid in the current mode we are in.
 * @param init
 * @param edit
 * @param solve
 * @param mode
 * @return boolean value
 */
bool check_mode(bool init, bool edit, bool solve, int mode);
/**
 * The function checks if a single char is a number between 0 or 9, if so it will return true.
 * @param ch
 * @return boolean value
 */
bool isNum(char ch);
/**
 * The function gets a string and checks if its a int by checking if each char in it is a num via the isNum func.
 * it will return true if so, else false.
 * @param str
 * @return boolean value
 */
bool isInt(char* str);
/**
 * The function gets a string and checks if its a float by checking if each char in it is a num via the isNum func.
 * And if a single dot exist in the number.
 * If it's in the correct format we will return true else false.
 * @param str
 * @return boolean value
 */
bool isFloat(char* str);
/**
 *  The function checks if the number that was read from the file is in the correct form using the
 *  fFix1,IfFix2, IfNotFix1,IfNotFix2 functions and will print informative message otherwise.
 *  It every check that was made is correct we will return true else false .
 * @param game_board
 * @param x
 * @param res0
 * @param res1
 * @param res2
 * @param col
 * @param row
 * @return boolean value
 */
bool fileToCells(puzzle* game_board, char* x, char* res0, char* res1, char* res2, int col, int row);
/**
 * The function checks if the number that was read is a single digit and if its fixed and returns true or false
 * according to it.
 * @param game_board
 * @param x
 * @param res0
 * @param col
 * @param row
 * @return boolean value
 */
bool IfFix1(puzzle* game_board, char* x, char* res0, int col, int row);
/**
 * The function checks if the number that was read is a double digit and if its fixed and returns true or false
 * according to it.
 * @param game_board
 * @param x
 * @param res0
 * @param res1
 * @param col
 * @param row
 * @return boolean value
 */
bool IfFix2(puzzle* game_board, char* x, char* res0, char* res1, int col, int row);
/**
 * The function checks if the number that was read is a single digit and if its not fixed and returns true or false
 * according to it.
 * @param game_board
 * @param x
 * @param res0
 * @param col
 * @param row
 * @return boolean value
 */
bool IfNotFix1(puzzle* game_board, char* x, char* res0, int col, int row);
/**
 * The function checks if the number that was read is a double digit and if its not fixed and returns true or false
 * according to it.
 * @param game_board
 * @param x
 * @param res0
 * @param res1
 * @param col
 * @param row
 * @return boolean value
 */
bool IfNotFix2(puzzle* game_board, char* x, char* res0, char* res1, int col, int row);
/**
 * The function return strtok on prt and delimiters strings only if its valid. If not, it's keep reading the file to buff untill it get EOF.
 * If the file gets EOF, the function return NULL.
 * according to it.
 * @param fptr
 * @param ptr
 * @param delimiters
 * @param buff
 * @return char* or NULL value
 */
char* strtokForFile(FILE* fptr, char* ptr, const char* delimiters, char buff[])	;
#endif
