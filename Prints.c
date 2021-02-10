#include "Prints.h"

void ParamError(int x, int size)
{
	printf("ERROR: %d is illegal, the number should be between 1 to %d\n", x, size);
}
void setParamZError(int z, int size)
{
	printf("ERROR: %d is illegal, the number should be between 0 to %d\n", z, size);
}
void printFixedError()
{
	printf("ERROR: this cell is fixed\n");
}
void printFullBoard()
{
	printf("The puzzle was solved successfully\n");
}
void printFullBoardNotValid()
{
	printf("Error! The solution is erroneous\n");
}
void printFuncError(char* func)
{
	printf("Error: %s has failed\n", func);
	exit(1);
}
void printInvalidCommand()
{
	printf("Error: invalid command\n");
}
void printWrongParams(int num)
{
	if (num >= 0)
	{
		printf("Error : The number of parameters is wrong! You must enter %d parameters\n", num);
	}
	else
	{
		printf("Error : The number of parameters is wrong! You may enter 0 or 1 parameters\n");
	}
}
void printFileError()
{
	printf("Error! file doesn't exist\n");
}
void printIllegalFormat()
{
	printf("Error! file should contain only digits and digits bigger than 0 with dots\n");
}
void printRangeError(int size)
{
	printf("Error! file should contain numbers from 0 to %d\n",size);
}
void printSingleDigitError()
{
	printf("Error! If digit<10, it shouldn't contain a zero before it\n");
}
void printTooManyDigits()
{
	printf("Error! Each cell in the board can contain a number with only 2 digits, and a dot after them if it's fixed\n");

}
void printShortError()
{
	printf("Error!The file is too short\n");
}
void printLongError()
{
	printf("Error! The file is too long\n");
}
void printTooBig()
{
	printf("Error! The rows and cols shold be between 0 to 5\n");
}
void printNotDigitError()
{
	printf("Error! The rows and cols shold be digits\n");
}
void printTooManyInALine()
{
	printf("Error! Too many values in the line\n");
}
void printNotEnoughInLine()
{
	printf("Error! Not enough values\n");
}
void printUnsolvableBoard()
{
	printf("Error! the board is unsolvable\n");
}
void printSolvableBoard()
{
	printf("The board is solvable\n");
}
void printInvalidBoard()
{
	printf("Error! the board is invalid\n");
}
void printValidBoard()
{
	printf("The board is valid\n");
}
void printNotRightMode(bool init, bool edit, bool solve)
{	
	printf("The function is available only in modes : ");
	if (init)
	{
		printf("Init ");
	}
	if (edit)
	{
		printf("Edit ");

	}
	if (solve)
	{
		printf("Solve");
	}
	printf("\n");
}
void printContainVal()
 {
	 printf("Error! this cell contains value\n");
 }
void printValHint(int val)
 {
	 printf("The value is: %d\n", val);
 }

void printNumOfSolve(int count)
{
	 printf("The number of solutions is: %d\n", count);
}

void printAutoFill(int i, int j,int  num)
 {
	 printf("The cell in row %d and col %d was autofilled to %d\n", i + 1, j + 1, num);
 }
void printInvalidThreshold()
 {
	 printf("This is an invalid Threshold the value should be between 0 and 1 \n");
 }
void printNotEnoughEmptyCells() 
 {
	 printf("Error! There are not enough empty cells\n");
 }
void printGenerationFailed()
 {
	 printf(" Error! The generation failed\n");
 }
void printAllLegalValues(int size, int x, int y, double*** prob)
 {
	 int k, count = 0;
	 printf("The possible values for cell %d %d is: \n", x, y);
	 for (k = 0; k < size; k++)
	 {
		 if (prob[x][y][k] > 0)
		 {
			 count++;
			 printf("%d with score of %f\n", k + 1, prob[x][y][k]);
		 }
	 }
	 if (count == 0)
	 {
		 printf("None\n");
	 }
 }
void printExitGame()
 {
	 printf("Exiting... Have a nice day\n");
 }

void printTooManyCharacters()
 {
	 printf("Too many characters were entered in a single line. You may not enter a line with more than 256 characters\n");
 }
void printPrompt()
 {
	 printf("Please enter command\n");
 }
void printNotIntParam()
{
	printf("Error! the parameters must be integers\n");
}
void printParam01()
{
	printf("Error! the parameters must be 0 or 1\n");
}
void printNotFloatParam()
{
	printf("Error! the parameter must be float\n");
}

void printNoMoreUndo()
{
	printf("Error! No more previous moves to undo\n");
}
void printNoMoreRedo()
{
	printf("Error! No more moves to redo\n");
}
void printWelcome()
{
	printf("Welcome to the Sudoku game By Adi and Benny! You are currently in init mode.\n");
}
 void printUndoRedo(int x, int y, int z)
{
	if (z != -1)
	{
		printf("The cell in row %d,column %d was changed to %d.\n", x+1, y+1, z);
	}
	else
	{
		printf("The cell in row %d,column %d was changed to %d.\n", x+1, y+1, 0);
	}
}
 void printValueBetween(char* of_value, int min, int max)
 {
	 printf("The value of the %s should be between %d to %d\n", of_value, min, max);
 }