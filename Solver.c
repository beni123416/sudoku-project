#include "Solver.h"
#include "Logic.h"


int LP(puzzle* game_board, double*** prob)
{
	GRBenv* env = NULL;
	GRBmodel* model = NULL;
	int size, length;
	int*** arr;
	int* ind;
	double* val;
	double* ub, * sol;
	char* vtype;
	int optimstatus;
	double *obj;
	int num,i, j, k, iStart, jStart, count;
	int tmp;
	int error = 0, flag = 0;
	size = game_board->size;
	arr = init_matVars(game_board);
	find_matVars(game_board, arr);
	length = numOfVars(arr, size);
	ub = (double*)malloc(sizeof(double) * length);
	sol = (double*)malloc(sizeof(double) * length);
	vtype = (char*)malloc(sizeof(char) * length);
	if (!ub || !sol || !vtype || !arr)
	{
		printFuncError("malloc");
	}
	/* coefficients  */
	obj = (double*)malloc(sizeof(double) * length);
	for (i = 0; i < length; i++)
	{
		obj[i] = 1.0 + rand()%game_board->size;
	}
	/* Create an empty model */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				tmp = arr[i][j][k];
				if (tmp > -1)
				{
						ub[tmp] =1.0;
						vtype[tmp] = GRB_CONTINUOUS;
				}
			}
		}
	}

	/* Create environment */
	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		flag = 3;
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		goto QUIT;
	}
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		flag = 3;
		goto QUIT;
	}

	/* Create new model */
	error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL);
	if (error)
	{
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		flag = 3;
		goto QUIT;
	}

	/* add variables to model */
	error = GRBaddvars(model, length, 0, NULL, NULL, NULL, obj, NULL, ub, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}
	error = GRBupdatemodel(model);
	if (error)
	{
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Each cell gets a value */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {

			ind = (int*)malloc(sizeof(int) * numOfCellsVars(arr, size, i, j));
			val = (double*)malloc(sizeof(double) * numOfCellsVars(arr, size, i, j));
			if (!ind || !val)
			{
				printFuncError("malloc");
			}

			num = 0;
			for (k = 0; k < size; k++) {
				tmp = arr[i][j][k];
				if (tmp > -1)
				{
					ind[num] = tmp;
					val[num] = 1.0;
					num++;
				}
			}

			error = GRBaddconstr(model, numOfCellsVars(arr, size, i, j), ind, val, GRB_EQUAL, 1.0, NULL);
			free(ind);
			free(val);
			if (error)
			{
				printf("ERROR %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				printf("I'm here1\n");
				flag = 3;
				goto QUIT;
			}
		}
	}


	/* Each value must appear once in each column */
	for (k = 0; k < size; k++) {
		for (j = 0; j < size; j++) {

			ind = (int*)malloc(sizeof(int) * numOfColsVars(arr, size, j, k)); /*compute how many vars represent this column*/
			val = (double*)malloc(sizeof(double) * numOfColsVars(arr, size, j, k));
			if (!ind || !val)
			{
				printFuncError("malloc");
			}

			num = 0;

			for (i = 0; i < size; i++) {
				tmp = arr[i][j][k];

				if (tmp > -1)
				{
					ind[num] = tmp;
					val[num] = 1.0;
					num++;
				}
			}

			error = GRBaddconstr(model, numOfColsVars(arr, size, j, k), ind, val, GRB_EQUAL, 1.0, NULL);
			free(ind);
			free(val);
			if (error)
			{
				printf("ERROR %d GRBaddconst2(): %s\n", error, GRBgeterrormsg(env));
				flag = 3;
				goto QUIT;
			}
		}
	}

	/* Each value must appear once in each row */
	for (k = 0; k < size; k++) {
		for (i = 0; i < size; i++) {
			ind = (int*)malloc(sizeof(int) * numOfRowsVars(arr, size, i, k));
			val = (double*)malloc(sizeof(double) * numOfRowsVars(arr, size, i, k));
			if (!ind || !val)
			{
				printFuncError("malloc");
			}

			num = 0;
			for (j = 0; j < size; j++) {
				tmp = arr[i][j][k];
				if (tmp > -1)
				{
					ind[num] = tmp;
					val[num] = 1.0;
					num++;
				}
			}

			error = GRBaddconstr(model, numOfRowsVars(arr, size, i, k), ind, val, GRB_EQUAL, 1.0, NULL);
			free(ind);
			free(val);
			if (error)
			{
				printf("ERROR %d GRBaddconstr3(): %s\n", error, GRBgeterrormsg(env));
				flag = 3;
				goto QUIT;
			}
		}
	}

	/* Each value must appear once in each block */
	for (k = 0; k < size; k++) {
		for (iStart = 0; iStart < game_board->rows; iStart++) {
			for (jStart = 0; jStart < game_board->cols; jStart++) {
				ind = (int*)malloc(sizeof(int) * numOfBlockVars(arr, game_board->rows, game_board->cols, iStart, jStart, k));
				val = (double*)malloc(sizeof(double) * numOfBlockVars(arr, game_board->rows, game_board->cols, iStart, jStart, k));
				if (!ind || !val)
				{
					printFuncError("malloc");
				}

				count = 0;
				for (i = iStart * game_board->cols; i < (iStart + 1) * game_board->cols; i++) {
					for (j = jStart * game_board->rows; j < (jStart + 1) * game_board->rows; j++) {
						tmp = arr[i][j][k];
						if (tmp > -1)
						{
							ind[count] = tmp;
							val[count] = 1.0;
							count++;
						}
					}
				}

				error = GRBaddconstr(model, numOfBlockVars(arr, game_board->rows, game_board->cols, iStart, jStart, k), ind, val, GRB_EQUAL, 1.0, NULL);
				free(ind);
				free(val);
				if (error)
				{
					printf("ERROR %d GRBaddconstr4(): %s\n", error, GRBgeterrormsg(env));
					flag = 3;
					goto QUIT;
				}
			}
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error)
	{
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	/* Write model to 'sudoku.lp' */
	error = GRBwrite(model, "sudoku.lp");
	if (error)
	{
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error)
	{
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	if (optimstatus == GRB_OPTIMAL)
		flag = flag;
	else if (optimstatus == GRB_INF_OR_UNBD)
		flag = 2;
	else
		flag = 2;

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL,obj);
	if (error)
	{
		printf("ERROR %d GRBgetdblattr(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}
	
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, length, sol);
	if (error)
	{
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	/* create the prob*/ 
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				tmp = arr[i][j][k];
				if (tmp != -1)
				{
					if (sol[tmp] > 0.0)
					{
						prob[i][j][k] = sol[tmp];
					}
				}
			}
		}
	}


QUIT:

	/* Free model */

	GRBfreemodel(model);

	/* Free environment */

	GRBfreeenv(env);

	/*free memory */

	freeILPAndLP(vtype, arr, ub, sol, size);
	return flag;

}

int ILP(puzzle* game_board)
{
	GRBenv* env = NULL;
	GRBmodel* model = NULL;
	int size, length;
	int*** arr;
	int* ind;
	double* val;
	double* lb, * sol;
	char* vtype;
	int optimstatus;
	double objval;
	int num, i, j, k, iStart, jStart, count;
	int tmp;
	int error = 0, flag = 0;
	size = game_board->size;
	arr = init_matVars(game_board);
	find_matVars(game_board, arr);
	length = numOfVars(arr, size);
	lb = (double*)malloc(sizeof(double) * length);
	sol = (double*)malloc(sizeof(double) * length);
	vtype = (char*)malloc(sizeof(char) * length);
	
	if (!lb || !sol || !vtype || !arr)
	{
		printFuncError("malloc");
		flag = 3;
		goto QUIT;
	}
	

	/* Create an empty model */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				tmp = arr[i][j][k];
				if (tmp > -1)
				{
					if (game_board->mat[i][j].val == k + 1)
					{
						lb[tmp] = 1;
					}
					else
					{
						lb[tmp] = 0;
					}
					vtype[tmp] = GRB_BINARY;
				}
			}
		}
	}

	/* Create environment */
	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		flag = 3;
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		goto QUIT;
	}
	
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		flag = 3;
		goto QUIT;
	}
	/* Create new model */
	error = GRBnewmodel(env, &model, "sudoku", length, NULL, lb, NULL, vtype, NULL);
	if (error)
	{
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		flag = 3;
		goto QUIT;
	}

	/* Each cell gets a value */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {

			ind = (int*)malloc(sizeof(int) * numOfCellsVars(arr, size, i, j));
			val = (double*)malloc(sizeof(double) * numOfCellsVars(arr, size, i, j));
			if (!ind || !val)
			{
				printFuncError("malloc");
				flag = 3;
				goto QUIT;
			}

			num = 0;
			for (k = 0; k < size; k++) {
				tmp = arr[i][j][k];
				if (tmp > -1)
				{
					ind[num] = tmp;
					val[num] = 1.0;
					num++;
				}
			}

			error = GRBaddconstr(model, numOfCellsVars(arr, size, i, j), ind, val, GRB_EQUAL, 1.0, NULL);
			free(ind);
			free(val);
			if (error)
			{
				printf("ERROR %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				flag = 3;
				goto QUIT;
			}
		}
	}


	/* Each value must appear once in each column */
	for (k = 0; k < size; k++) {
		for (j = 0; j < size; j++) {

			ind = (int*)malloc(sizeof(int) * numOfColsVars(arr, size, j, k)); /*compute how many vars represent this column*/
			val = (double*)malloc(sizeof(double) * numOfColsVars(arr, size, j, k));
			if (!ind || !val)
			{
				printFuncError("malloc");
				flag = 3;
				goto QUIT;
			}

			num = 0;

			for (i = 0; i < size; i++) {
				tmp = arr[i][j][k];

				if (tmp > -1)
				{
					ind[num] = tmp;
					val[num] = 1.0;
					num++;
				}
			}

			error = GRBaddconstr(model, numOfColsVars(arr, size, j, k), ind, val, GRB_EQUAL, 1.0, NULL);
			free(ind);
			free(val);
			if (error)
			{
				printf("ERROR %d GRBaddconst2(): %s\n", error, GRBgeterrormsg(env));
				flag = 3;
				goto QUIT;
			}
		}
	}

	/* Each value must appear once in each row */
	for (k = 0; k < size; k++) {
		for (i = 0; i < size; i++) {
			ind = (int*)malloc(sizeof(int) * numOfRowsVars(arr, size, i, k));
			val = (double*)malloc(sizeof(double) * numOfRowsVars(arr, size, i, k));
			if (!ind || !val)
			{
				printFuncError("malloc");
				flag = 3;
				goto QUIT;
			}

			num = 0;
			for (j = 0; j < size; j++) {
				tmp = arr[i][j][k];
				if (tmp > -1)
				{
					ind[num] = tmp;
					val[num] = 1.0;
					num++;
				}
			}

			error = GRBaddconstr(model, numOfRowsVars(arr, size, i, k), ind, val, GRB_EQUAL, 1.0, NULL);
			free(ind);
			free(val);
			if (error)
			{
				printf("ERROR %d GRBaddconstr3(): %s\n", error, GRBgeterrormsg(env));
				flag = 3;
				goto QUIT;
			}
		}
	}

	/* Each value must appear once in each block */
	for (k = 0; k < size; k++) {
		for (iStart = 0; iStart < game_board->rows; iStart++) {
			for (jStart = 0; jStart < game_board->cols; jStart++) {
				ind = (int*)malloc(sizeof(int) * numOfBlockVars(arr, game_board->rows, game_board->cols, iStart, jStart, k));
				val = (double*)malloc(sizeof(double) * numOfBlockVars(arr, game_board->rows, game_board->cols, iStart, jStart, k));
				if (!ind || !val)
				{
					printFuncError("malloc");
					flag = 3;
					goto QUIT;
				}

				count = 0;
				for (i = iStart * game_board->cols; i < (iStart + 1) * game_board->cols; i++) {
					for (j = jStart * game_board->rows; j < (jStart + 1) * game_board->rows; j++) {
						tmp = arr[i][j][k];
						if (tmp > -1)
						{
							ind[count] = tmp;
							val[count] = 1.0;
							count++;
						}
					}
				}

				error = GRBaddconstr(model, numOfBlockVars(arr, game_board->rows, game_board->cols, iStart, jStart, k), ind, val, GRB_EQUAL, 1.0, NULL);
				free(ind);
				free(val);
				if (error)
				{
					printf("ERROR %d GRBaddconstr4(): %s\n", error, GRBgeterrormsg(env));
					flag = 3;
					goto QUIT;
				}
			}
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error)
	{
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	/* Write model to 'sudoku.lp' */
	error = GRBwrite(model, "sudoku.lp");
	if (error)
	{
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error)
	{
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	if (optimstatus == GRB_OPTIMAL)
		flag = flag;
	else if (optimstatus == GRB_INF_OR_UNBD)
		flag = 2;
	else
		flag = 2;

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error)
	{
		/*printf("ERROR %d GRBgetdblattr(): %s\n", error, GRBgeterrormsg(env));*/
		flag = 2;
		goto QUIT;
	}

	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, length, sol);
	if (error)
	{
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		flag = 2;
		goto QUIT;
	}

	/* create the board */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				tmp = arr[i][j][k];
				if (tmp != -1)
				{
					if (sol[tmp] == 1.0)
					{
						setSol(game_board, i, j, k);
					}
				}
			}
		}
	}


QUIT:

	/* Free model */

	GRBfreemodel(model);

	/* Free environment */

	GRBfreeenv(env);

	/*free memory */

	freeILPAndLP(vtype, arr, lb, sol, size);

	return flag; /* 0 - solveable, 1 - not solveable, 2,3- error*/

}

int FullCells(puzzle* game_board)
{
	int i, j, count = 0;
	for (i = 0; i < game_board->size; i++)
	{
		for (j = 0; j < game_board->size; j++)
		{
			if (game_board->mat[i][j].val != -1)
			{
				count++;
			}
		}
	}
	return count;
}

int*** init_matVars(puzzle* game_board)
{
	int i, j, k;
	int*** legalVars = (int***)malloc(game_board->size * sizeof(int**));
	if (!legalVars)
	{
		printFuncError("malloc");
		return NULL;
	}
	for (i = 0; i < game_board->size; i++)
	{
		legalVars[i] = (int**)malloc(game_board->size * sizeof(int*));

		if (!legalVars[i])
		{
			printFuncError("malloc");
			return NULL;
		}
	}
	for (j = 0; j < game_board->size; j++)
	{
		for (k = 0; k < game_board->size; k++)
		{
			legalVars[j][k] = (int*)malloc(game_board->size * sizeof(int));
			if (!legalVars[j][k])
			{
				printFuncError("malloc");
				return NULL;
			}
		}
	}
	return legalVars;
}

void find_matVars(puzzle* game_board, int*** ligalVars)
{
	int i, j, k, count = 0;
	for (i = 0; i < game_board->size; i++)
	{
		for (j = 0; j < game_board->size; j++)
		{
			if (game_board->mat[i][j].val == -1)
			{
				for (k = 0; k < game_board->size; k++)
				{

					if (legalNum(game_board, k + 1, i, j,false))
					{
						ligalVars[i][j][k] = count;
						count++;
					}
					else
					{
						ligalVars[i][j][k] = -1;
					}
				}
			}
			else
			{
				for (k = 0; k < game_board->size; k++)
				{
					if (k + 1 == game_board->mat[i][j].val)
					{
						ligalVars[i][j][k] = count;
						count++;
					}
					else
					{
						ligalVars[i][j][k] = -1;
					}
				}
			}


		}
	}

}

int numOfVars(int*** ligalVars, int size)
{
	int i, j, k, count = 0;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			for (k = 0; k < size; k++)
			{
				if (ligalVars[i][j][k] > -1)
				{
					count++;
				}
			}
		}
	}
	return count;
}

int numOfCellsVars(int*** ligalVars, int size, int i, int j)
{
	int k, count = 0;
	for (k = 0; k < size; k++)
	{
		if (ligalVars[i][j][k] > -1)
		{
			count++;
		}
	}
	return count;
}

int numOfRowsVars(int*** ligalVars, int size, int i, int k)
{
	int j, count = 0;
	for (j = 0; j < size; j++)
	{
		if (ligalVars[i][j][k] > -1)
		{
			count++;
		}
	}
	return count;
}

int numOfColsVars(int*** ligalVars, int size, int j, int k)
{
	int i, count = 0;
	for (i = 0; i < size; i++)
	{
		if (ligalVars[i][j][k] > -1)
		{
			count++;
		}
	}
	return count;
}

int numOfBlockVars(int*** ligalVars, int n,int m, int iStart, int jStart,int k)
{
	int i, j, count=0;
	for (i = iStart * m; i < (iStart + 1) * m; i++) 
	{
		for (j = jStart * n; j < (jStart + 1) * n; j++)
		{
			if (ligalVars[i][j][k] > -1)
			{
				count++;
			}
		}
	}
	return count;
}

void setSol(puzzle* game_board, int i, int j, int k)
{
	game_board->solMat[i][j].val = k+1;
}

int backTracking_exh(puzzle *game_board)
{
	int num,i,l ,j,k,count,numOfEmpty;
	int* arr;
	struct Stack* stack; 
	int* index = emptyCell(game_board);
	int* fullCell = emptyCell(game_board);
	int*** matVars = init_matVars(game_board);
	count = 0;
	numOfEmpty = 0;
	stack =createStack(game_board->size * game_board->size * game_board->size);
	numOfEmpty++;
	for (i = 0; i < game_board->size; i++)
	{
		for (j = 0; j < game_board->size; j++)
		{
			for (k = 0; k < game_board->size; k++)
			{
				matVars[i][j][k] = 1;
			}
		}
	}
	while (index[0] != -1 && index[1] != -1)
	{
		for (num = 1; num < game_board->size + 1; num++)
		{
			if (matVars[index[0]][index[1]][num-1] !=0  && legalNum(game_board,num, index[0], index[1],false))
			{
				game_board->mat[index[0]][index[1]].val = num;
				pushStack(stack, index[0], index[1], num);
				break;
			}
		}
		free(fullCell);
		fullCell = emptyCell(game_board);
		if (fullCell[0] == -1 && fullCell[1] == -1)
		{
			count++;
			if (isEmpty(stack)|| numOfEmpty==1)
			{
				freeBackTrack(index, fullCell, matVars, stack, game_board->size);
				return count;
			}
			arr=pop(stack);
			game_board->mat[arr[0]][arr[1]].val = -1;
			for (l = 0; l < game_board->size; l++)
			{
				matVars[arr[0]][arr[1]][l] = 1;
			}
			matVars[arr[0]][arr[1]][arr[2]-1] = 0;

		}
		
		if (game_board->mat[index[0]][index[1]].val == -1 && !isEmpty(stack) && fullCell[0] != -1 && fullCell[1] != -1)
		{
            for (l = 0; l < game_board->size; l++)
            {
                matVars[index[0]][index[1]][l] = 1;
            }
			arr = pop(stack);
			matVars[arr[0]][arr[1]][arr[2]-1] = 0;
			game_board->mat[arr[0]][arr[1]].val = -1;
			while (finishVal(game_board,matVars,arr[0],arr[1]) && !isEmpty(stack))
			{
				for (l = 0; l < game_board->size; l++)
				{
					matVars[arr[0]][arr[1]][l] = 1;
				}
				arr = pop(stack); 
				matVars[arr[0]][arr[1]][arr[2]-1] = 0;
				game_board->mat[arr[0]][arr[1]].val = -1;
			}
			if (isEmpty(stack) && finishVal(game_board, matVars, arr[0], arr[1]))
			{
				freeBackTrack(index, fullCell, matVars, stack, game_board->size);
				return count;
			}
		}
		free(index);
		index = emptyCell(game_board);
		numOfEmpty++;
	}
	freeBackTrack(index, fullCell, matVars, stack, game_board->size);
	return count;
}

int* emptyCell(puzzle* game_board)
{
	int i, j;
	int* index = (int*)malloc(2 * sizeof(int));
	if (!index)
	{
		printFuncError("malloc");
		return NULL;
	}
	for (i = 0; i < game_board->size; i++)
	{
		for (j = 0; j < game_board->size; j++)
		{
			if (game_board->mat[i][j].val == -1)
			{
				index[0] = i;
				index[1] = j;
				return index;
			}
		}
	}
	index[0] = index[1] = -1;
	return index;
}

bool finishVal(puzzle* game_board, int*** matVars, int i, int j)
{
	int k;
	for (k = 0; k < game_board->size; k++)
	{
		if (matVars[i][j][k] != 0 && legalNum(game_board, k+1, i, j,false))
		{
			return false;
		}
	}
	return true;
}

void freeBackTrack(int* index,int *fullCell, int*** matVars, struct Stack* stack,int size)
{
	int i,j;
	free(index);
	free(fullCell);
	freeStack(stack);

	for (i = 0; i <size; i++)
	{
		for (j = 0; j <size; j++)
		{
			free(matVars[i][j]);
		}
		free(matVars[i]);
	}
	free(matVars);
	
}
 
void freeILPAndLP(char* vtype, int*** arr, double* lb, double* sol, int size)
{
	int i, j;
	if (vtype)
	{
		free(vtype);
	}
	if (lb)
	{
		free(lb);
	}
	if (sol)
	{
		free(sol);
	}
	if (!arr)
	{
		return;
	}
	for (i = 0; i < size; i++)
	{
		if (!arr[i])
		{
			continue;
		}
		for (j = 0; j < size; j++)
		{
			if (arr[i][j])
			{
				free(arr[i][j]);
			}
		}
		free(arr[i]);
	}
	free(arr);
}
